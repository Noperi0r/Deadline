// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon\WeaponThrowBase.h"
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "Kismet\KismetMathLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "Components\SplineComponent.h"
#include "Components\BoxComponent.h"
#include "Player\ZombiePlayerController.h"
#include "InventoryComponent.h"
#include "Components\AudioComponent.h"
#include "GeometryCollection\GeometryCollectionComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Materials\MaterialInstanceDynamic.h"

AWeaponThrowBase::AWeaponThrowBase()
{
	// MUST BE SET IN CHILD BP 
	ThrowingTag = EThrowingTag::NONE;

	ThrowingMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection Component"));
	ThrowingMesh->SetupAttachment(GetRootComponent());
	ThrowingMesh->bCastDynamicShadow = false;
	ThrowingMesh->CastShadow = false;
	ThrowingMesh->SetSimulatePhysics(false);
	ThrowingMesh->SetNotifyBreaks(true);

	AttackSound->SetupAttachment(ThrowingMesh);

	Montage_Trigger = CreateDefaultSubobject<UAnimMontage>(TEXT("Montage Trigger"));

	ItemTag = EWeaponTag::THROW;

	ThrowPower = 2500.0f;

	ThrowLoudness = 100.0f;
	ThrowLoudnessRange = 500.0f;

	NoiseEmitter = CreateDefaultSubobject<UBaseNoiseEmitterComponent>(TEXT("Noise Emitter"));
}


EThrowingTag AWeaponThrowBase::GetThrowingTag()
{
	return ThrowingTag;
}

void AWeaponThrowBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	if (Player->bDoingAttack)
	{
		PerformTrace();
		if (NoiseDisplayer->IsHidden())
			NoiseDisplayer->SetActorHiddenInGame(false);
	}
	else
	{
		if(!NoiseDisplayer->IsHidden())
			NoiseDisplayer->SetActorHiddenInGame(true);
	}

	if (Player->bReadyForThrow)
	{
		if (Player->CanPlayerThrow())
		{
			Player->bReadyForThrow = false; // bReadyForThrow = false;
			Player->SetPlayerThrowFlag(false);
			Launch();
		}
	}
}

void AWeaponThrowBase::BeginPlay()
{
	Super::BeginPlay();

	//ThrowingMesh->OnComponentHit.AddDynamic(this, &AWeaponThrowBase::OnHit);
	ThrowingMesh->OnChaosBreakEvent.AddDynamic(this, &AWeaponThrowBase::ChaosBreakHandler);

	SpawnNoiseSphere();
}

void AWeaponThrowBase::Attack()
{
	if (!Player) return;
	
	Player->SetPlayerThrowFlag(false);

	Player->bDoingAttack = true;  // bDoingAttack is activated during montage is on
	//Player->bCannotAction = true;

	// TEMP 24.10.07 TODO: Uncomment this
	//OnTriggerMontageEnded(nullptr, false);
	//Player->PlayerAnimInstance->Montage_Play(Montage_Trigger);
	//if (!Player->PlayerAnimInstance->WeaponActionEndDelegate.IsBound())
	//{
	//	Player->PlayerAnimInstance->WeaponActionEndDelegate.BindUFunction(this, FName("OnTriggerMontageEnded"));
	//	Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->WeaponActionEndDelegate, Montage_Trigger);
	//}
	Player->bReadyForThrow = true;
}

void AWeaponThrowBase::Launch()
{
	bHit = false;	

	// Reset player state
	Player->bDoingAttack = false;
	Player->SetPlayerThrowFlag(false);

	//ThrowingMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ThrowingMesh->SetSimulatePhysics(true); // Enable free fall
	ThrowingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Set projectile state to be launched
	const FVector ThrowLocation = Player->ThrowLocation->GetComponentLocation(); // Needs to be called before detachfromcomponent but why
	//SetActorLocation(ThrowLocation);
	
	ThrowingMesh->SetWorldLocation(ThrowLocation);
	//DrawDebugSphere(GetWorld(), ThrowingMesh->GetComponentLocation(), 10, 16, FColor::Blue, true, 3.0f);

	const FVector ForwardVec = UKismetMathLibrary::GetForwardVector(Player->GetControlRotation());

	ThrowingMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	ThrowingMesh->AddImpulse(ForwardVec * Player->ThrowingForce, NAME_None, true);

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
	Player->PlayerInventory->Inventory_ClearWeapon(THROWINDEX, (int32)ThrowingTag - 1);
	Player->bThrowingLaunched = true;

	int32 Num =  Player->PlayerInventory->CheckThrowingNum(ThrowingTag);
	if (Num <= 0)
	{
		Player->SetNextWeaponIndex(THROWINDEX);

		if (!Player->PlayerInventory->bNoThrowings) // Has throwings of other type compared to the current one
			Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
		else // Has no throwings at all
			Player->WeaponChange(MELEEINDEX, Player->RecentMeleeWeaponIndex);

	}
	else // Has more throwings with the same type as current one
	{
		Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
	}
	Player = nullptr; 

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() {Destroy(); UE_LOG(LogTemp, Warning, TEXT("Lambda Called")) }), 5.0f, false);
}

bool AWeaponThrowBase::CanAttack()
{
	// CHANGE 
	if (!Player) return false;
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
	PathParams.MaxSimTime = 10.0f;
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
		else Player->bReadyForThrow = true; //bReadyForThrow = true; // Condition for the situation where montage is ended but player is holding mouse
	}
}

void AWeaponThrowBase::SpawnNoiseSphere()
{
	NoiseDisplayer = GetWorld()->SpawnActor(NoiseDisplayRef);
	UStaticMeshComponent* NoiseMesh = Cast<UStaticMeshComponent>(NoiseDisplayer->GetRootComponent());
	UMaterialInterface* NoiseMat = NoiseMesh->GetMaterial(0);
	
	checkf(NoiseMat, TEXT("No material in noisedisplayer"));
	NoiseMatInst = UMaterialInstanceDynamic::Create(NoiseMat, this);
	NoiseMesh->SetMaterial(0, NoiseMatInst);

	FHashedMaterialParameterInfo ParaInfo;
	ParaInfo.Name = FScriptName(TEXT("Alpha"));
	NoiseMatInst->GetScalarParameterValue(ParaInfo, NoiseColAlpha);

	OriginalNoiseColAlpha = NoiseColAlpha;

	float ApproxSphereRadius = 100.0f / 2.0f;
	float TargetScale = WeaponLoudnessRange / ApproxSphereRadius;
	NoiseMesh->SetWorldScale3D(FVector(TargetScale, TargetScale, TargetScale));

	NoiseDisplayer->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NoiseDisplayer->SetActorHiddenInGame(true);

	// After chaos break 
	//NoiseMesh->SetWorldLocation(Pos);
	//GetWorldTimerManager().SetTimer(NoiseDisplayTimer,
	//								FTimerDelegate::CreateUObject(this, &AWeaponThrowBase::UpdateNoiseSphereAlpha, NoiseDisplayer),
	//								GetWorld()->GetDeltaSeconds(), true);
}

void AWeaponThrowBase::UpdateNoiseSphereAlpha(AActor* DisplayObj)
{
	if (NoiseDisplayer->IsHidden()) NoiseDisplayer->SetActorHiddenInGame(false);

	//UE_LOG(LogTemp, Warning, TEXT(" ALPH %f"), NoiseColAlpha);
	UE_LOG(LogTemp, Warning, TEXT(" WHAT %d"), NoiseDisplayer->IsHidden());

	NoiseColAlpha = FMath::Lerp(NoiseColAlpha, 0, 0.1f);
	NoiseMatInst->SetScalarParameterValue(FName("Alpha"), NoiseColAlpha);

	if (NoiseColAlpha < 0.01f)
	{
		GetWorldTimerManager().ClearTimer(NoiseDisplayTimer);
		DisplayObj->Destroy();
	}
}

void AWeaponThrowBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
	UpdatePlayer();
}

void AWeaponThrowBase::MakeWeaponNoise(const FVector& Location)
{
	NoiseEmitter->Execute_MakeEnvironmentNoise(NoiseEmitter, nullptr, WeaponLoudness, Location, WeaponLoudnessRange);
	//DrawDebugSphere(GetWorld(), Location, WeaponLoudnessRange, 12, FColor::Green, false, 1.0f);
}

void AWeaponThrowBase::SubAction()
{
	if (bHit || !Player) return;
	
	if (Player->bReadyForThrow)
	{
		Player->bDoingAttack = false;
		Player->bReadyForThrow = false;
		Player->bCanThrow = false;

		for (USplineMeshComponent* Element : (Player->ThrowSplineMeshes))
		{
			Element->DestroyComponent();
		}
		Player->ThrowSplineMeshes.Empty();
		Player->ThrowPathSpline->ClearSplinePoints(true);
		Player->ThrowEndSphere->SetVisibility(false);
	}
	//Player->PlayerAnimInstance->Montage_Stop(0.3f);
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

			NoiseDisplayer->SetActorLocation(Paths[i].Location);
		}
	}
}

// Deprecated 
void AWeaponThrowBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHit) return;
	bHit = true;

	UE_LOG(LogTemp, Warning, TEXT("ON HIT ON HIT ON HIT ON HIT"));


	if (AttackSound)
		AttackSound->Play();
	
	if (!Player) return;
	AZombiePlayerController* PlayerController = Cast<AZombiePlayerController>(Player->Controller);
	if(!PlayerController) return;

	PlayerController->OnNoiseDelegate.Broadcast(ThrowLoudness, ThrowingMesh->GetComponentLocation(), ThrowLoudnessRange);
}

void AWeaponThrowBase::ChaosBreakHandler(const FChaosBreakEvent& BreakEvent)
{
	if (bHit) return;
	bHit = true;

	//DrawDebugSphere(GetWorld(), BreakEvent.Location, 10, 16, FColor::Red, true, 3.0f);
	
	UE_LOG(LogTemp, Warning, TEXT("CHAOS BREAK HANDLER"));

	if (AttackSound)
		AttackSound->Play();

	MakeWeaponNoise(BreakEvent.Location);

	NoiseDisplayer->SetActorHiddenInGame(false);
	NoiseDisplayer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	NoiseDisplayer->SetActorLocation(BreakEvent.Location);

	GetWorldTimerManager().SetTimer(NoiseDisplayTimer,
								FTimerDelegate::CreateUObject(this, &AWeaponThrowBase::UpdateNoiseSphereAlpha, NoiseDisplayer),
								GetWorld()->GetDeltaSeconds(), true);

	//DrawDebugSphere(GetWorld(), BreakEvent.Location, WeaponLoudnessRange, 32, FColor::Blue, true, 3.0f);
}


