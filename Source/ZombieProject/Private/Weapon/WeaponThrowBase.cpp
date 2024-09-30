// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon\WeaponThrowBase.h"
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "Kismet\KismetMathLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "Components\SplineComponent.h"
#include "Player/ZombiePlayerController.h"
#include "InventoryComponent.h"
#include "Components\AudioComponent.h"
#include "GeometryCollection\GeometryCollectionComponent.h"

AWeaponThrowBase::AWeaponThrowBase()
{
	// MUST BE SET IN CHILD BP 
	ThrowingTag = EThrowingTag::NONE;

	ThrowingMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection Component"));
	ThrowingMesh->SetupAttachment(GetRootComponent());
	ThrowingMesh->bCastDynamicShadow = false;
	ThrowingMesh->CastShadow = false;
	ThrowingMesh->SetSimulatePhysics(false);

	AttackSound->SetupAttachment(ThrowingMesh);


	Montage_Trigger = CreateDefaultSubobject<UAnimMontage>(TEXT("Montage Trigger"));

	ItemTag = EWeaponTag::THROW;

	ThrowPower = 2500.0f;

	ThrowLoudness = 100.0f;
	ThrowLoudnessRange = 500.0f;
}


EThrowingTag AWeaponThrowBase::GetThrowingTag()
{
	return ThrowingTag;
}

void AWeaponThrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	/*if (Player->bDoingAttack)
	{
		PerformTrace();
	}*/

	if (bReadyForThrow)
	{
		if (Player->CanPlayerThrow())
		{
			bReadyForThrow = false;
			Player->SetPlayerThrowFlag(false);
			Launch();
		}
	}
}

void AWeaponThrowBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponThrowBase::Attack()
{
	if (!Player) return;
	
	Player->SetPlayerThrowFlag(false);

	Player->bDoingAttack = true;  // bDoingAttack is activated during montage is on
	Player->bCannotAction = true;
	
	Player->PlayerAnimInstance->Montage_Play(Montage_Trigger);
	if (!Player->PlayerAnimInstance->WeaponActionEndDelegate.IsBound())
	{
		Player->PlayerAnimInstance->WeaponActionEndDelegate.BindUFunction(this, FName("OnTriggerMontageEnded"));
		Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->WeaponActionEndDelegate, Montage_Trigger);
	}
}

void AWeaponThrowBase::Launch()
{
	bHit = false;	

	// Reset player state
	Player->bDoingAttack = false;
	Player->SetPlayerThrowFlag(false);

	ThrowingMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ThrowingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ThrowingMesh->SetSimulatePhysics(true); // Enable free fall
	
	// Set projectile state to be launched
	const FVector ThrowLocation = Player->ThrowLocation->GetComponentLocation(); // Needs to be called before detachfromcomponent but why
	//SetActorLocation(ThrowLocation);
	
	ThrowingMesh->SetWorldLocation(ThrowLocation);

	const FVector ForwardVec = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());

	ThrowingMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	ThrowingMesh->AddImpulse(ForwardVec * Player->ThrowingForce, NAME_None, true);
	ThrowingMesh->OnComponentHit.AddDynamic(this, &AWeaponThrowBase::OnHit);

	// Clear Throw Spline, Meshes
	for (USplineMeshComponent* Element : (Player->ThrowSplineMeshes))
	{
		Element->DestroyComponent();
	}
	Player->ThrowSplineMeshes.Empty();
	Player->ThrowPathSpline->ClearSplinePoints(true);
	Player->ThrowEndSphere->SetVisibility(false);

	// Play montage
	Player->PlayerAnimInstance->Montage_Play(Montage_Attack);
	if (!Player->PlayerAnimInstance->AttackEndDelegate.IsBound())
	{
		Player->PlayerAnimInstance->AttackEndDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
		Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->AttackEndDelegate, Montage_Attack);
	}
}

void AWeaponThrowBase::UpdatePlayer()
{
	//int8 Num_CurrentThrowing = Player->PlayerInventory->GetThrowingNum(ThrowingTag) - 1;
	//Num_CurrentThrowing = Num_CurrentThrowing > 0 ? Num_CurrentThrowing : 0;

	//Player->PlayerInventory->SetThrowingNum(ThrowingTag, Num_CurrentThrowing);
	
	Player->PlayerInventory->Inventory_ClearWeapon(THROWINDEX, (int32)ThrowingTag - 1);

	int32 Num =  Player->PlayerInventory->CheckThrowingNum(ThrowingTag);
	if (Num <= 0)
	{
		Player->SetNextWeaponIndex(THROWINDEX);

		if (!Player->PlayerInventory->bNoThrowings)
			Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
		else
			Player->WeaponChange(MELEEINDEX, Player->RecentMeleeWeaponIndex);
	}
	else
	{
		Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
	}

	Player = nullptr;
}

bool AWeaponThrowBase::CanAttack()
{
	// CHANGE 
	return !Player->bDoingAttack;
}

void AWeaponThrowBase::PerformTrace()
{
	// Clear Throw Spline, Meshes
	for (USplineMeshComponent* Element : (Player->ThrowSplineMeshes))
	{
		Element->DestroyComponent();
	}
	Player->ThrowSplineMeshes.Empty();
	Player->ThrowPathSpline->ClearSplinePoints(true);
	
	FPredictProjectilePathParams PathParams;
	FPredictProjectilePathResult Result;

	PathParams.StartLocation = Player->ThrowLocation->GetComponentLocation();
	PathParams.LaunchVelocity = Player->ThrowingForce * UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());
	PathParams.bTraceWithCollision = true;
	PathParams.ProjectileRadius = 5.0f;
	PathParams.MaxSimTime = 1.0f;
	PathParams.bTraceWithChannel = true;
	PathParams.TraceChannel = ECollisionChannel::ECC_WorldStatic;
	PathParams.SimFrequency = 30.0f;
	PathParams.OverrideGravityZ = 0;
	PathParams.DrawDebugType = EDrawDebugTrace::None;

	UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, Result);
	UpdateSpline(Result);
}

void AWeaponThrowBase::OnTriggerMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Player)
	{
		Player->bCannotAction = false;
		//Player->bDoingAttack = false;
		//Player->SetPlayerThrowFlag(true);
		Player->PlayerAnimInstance->WeaponActionEndDelegate.Unbind();

		// Launch projectile if player did button up during montage
		if (Player->CanPlayerThrow()) Launch();
		else bReadyForThrow = true; // Condition for the situation where montage is ended but player is holding mouse
	}
}

void AWeaponThrowBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
	UpdatePlayer();
}

void AWeaponThrowBase::SubAction()
{
	UE_LOG(LogTemp, Warning, TEXT("SubAction Throw Working"));
}

void AWeaponThrowBase::ConvertPlayerController()
{
	// TODO: Change Controller Casting with this
}

void AWeaponThrowBase::UpdateSpline(const FPredictProjectilePathResult& PathResult)
{
	Player->ThrowPathSpline->ClearSplinePoints(true);

	if (!Player->ThrowEndSphere->IsVisible()) Player->ThrowEndSphere->SetVisibility(true);

	TArray Paths = PathResult.PathData;
	for (int i = 0; i < Paths.Num(); ++i)
	{
		//FVector Point = Player->GetActorLocation() + Paths[i].Location;
		//FVector Point = Player->ThrowLocation->GetComponentLocation() + Paths[i].Location;
		FVector Point = Paths[i].Location;
		Player->ThrowPathSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local);
		
		if (i == Paths.Num() - 1)
		{
			Player->ThrowPathSpline->SetSplinePointType(Paths.Num() - 1, ESplinePointType::CurveClamped, true);
			Player->ThrowEndSphere->SetWorldLocation(Paths[i].Location);
		}
	}
}

void AWeaponThrowBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ON HIT ON HIT ON HIT ON HIT"));

	// TODO: Change loudness to WeaponLoudness in weapon base class
	// TODO: Use Controller variable instead of PlayerController maybe

	if (bHit) return;
	bHit = true;

	if (AttackSound)
	{
		AttackSound->Play();
		UE_LOG(LogTemp, Warning, TEXT("ATTACKSOUND ON"));

	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ATTACKSOUND OFF"));
	
	if (!Player) return;
	AZombiePlayerController* PlayerController = Cast<AZombiePlayerController>(Player->Controller);
	if(!PlayerController) return;

	PlayerController->OnNoiseDelegate.Broadcast(ThrowLoudness, ThrowingMesh->GetComponentLocation(), ThrowLoudnessRange);
}


