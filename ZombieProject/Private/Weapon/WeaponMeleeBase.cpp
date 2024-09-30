// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon\WeaponMeleeBase.h"
#include "Engine\StaticMeshSocket.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\KismetArrayLibrary.h"
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "Enemy\ZombieBase.h"
#include "Player\ZombiePlayerController.h"
#include "Components\AudioComponent.h"

AWeaponMeleeBase::AWeaponMeleeBase() 
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee Weapon Mesh"));
	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetOnlyOwnerSee(false);

	// Setup Attack property
	MaxAttackNumber = 4;

	ItemTag = EWeaponTag::MELEE;
	
	Montage_Attack2 = CreateDefaultSubobject<UAnimMontage>(TEXT("Montage Attack 2"));
	Montage_Attack3 = CreateDefaultSubobject<UAnimMontage>(TEXT("Montage Attack 3"));
	Montage_Attack4 = CreateDefaultSubobject<UAnimMontage>(TEXT("Montage Attack 4"));

	Damage = 100.f;

	WeaponLoudness = 35.0f;
	WeaponLoudnessRange = 300000.0f;

	AttackSound->SetupAttachment(WeaponMesh, FName("S_SoundLocation"));
}


void AWeaponMeleeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player && bCollisionOn) PerformTrace();
}

void AWeaponMeleeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponMeleeBase::Attack()
{
	if (!Player) return;

	UE_LOG(LogTemp, Warning, TEXT("SwingCount: %d"), Player->SwingCount);
	switch (Player->SwingCount)
	{
		case 1:
			Player->PlayerAnimInstance->Montage_Play(Montage_Attack);

			Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
			Player->PlayerAnimInstance->AttackEndDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
			Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->AttackEndDelegate, Montage_Attack);
			break;

		case 2:
			Player->PlayerAnimInstance->Montage_Play(Montage_Attack2);

			Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
			Player->PlayerAnimInstance->AttackEndDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
			Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->AttackEndDelegate, Montage_Attack2);
			break;

		case 3:
			Player->PlayerAnimInstance->Montage_Play(Montage_Attack3);

			Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
			Player->PlayerAnimInstance->AttackEndDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
			Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->AttackEndDelegate, Montage_Attack3);
			break;

		case 4:
			Player->PlayerAnimInstance->Montage_Play(Montage_Attack4);

			Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
			Player->PlayerAnimInstance->AttackEndDelegate.BindUFunction(this, FName("OnAttackMontageEnded"));
			Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->AttackEndDelegate, Montage_Attack4);
			break;

	}

	if (Player->SwingCount >= MaxAttackNumber) Player->SwingCount = 0;
}

void AWeaponMeleeBase::SubAction()
{
	SubAction_Internal();
}

void AWeaponMeleeBase::SubAction_Internal_Implementation()
{
	// Check if can execute
	if (!Player->bCanExecute) return;
	if (Player->bOnExecute) return;
	if (Player->ExecuteMontages.Num() <= 0) return;
	Player->bOnExecute = true;
	
	if(Player->bOnCrouched)
	{
		AZombiePlayerController* PlayerController = Cast<AZombiePlayerController>(Player->Controller);
		PlayerController->DoCrouch();
	}

	const AZombieBase* ExecutedZombie = Cast<AZombieBase>(Player->TracedActor);
	const FVector TargetLocation =  ExecutedZombie->GetMesh()->GetBoneLocation("head",EBoneSpaces::WorldSpace);
	//FVector PlayerRayLocation = Player->Camera->GetComponentLocation();

	///const FVector PlayerForwardVector = UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
	//const FRotator RotationToForward = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, PlayerRayLocation+PlayerForwardVector*Player->ExecutableDistance);
	//Player->SetActorRotation(RotationToForward);
	//const FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, TargetLocation);
	//Player->SetActorRotation(RotationToTarget);
	
	Player->Camera->bUsePawnControlRotation = false;
	// Do Execution
	FVector ExecutionLocation = TargetLocation - UKismetMathLibrary::GetForwardVector(ExecutedZombie->GetActorRotation()) * Player->ExecutableDistance;
	ExecutionLocation.Z = Player->GetActorLocation().Z;
	Player->MotionWarping->AddOrUpdateWarpTargetFromLocation("ExecutionLocation",ExecutionLocation);
	ExecuteMontageIdx = FMath::RandRange(0, Player->ExecuteMontages.Num() - 1);
	Player->PlayerAnimInstance->Montage_Play(Player->ExecuteMontages[ExecuteMontageIdx]);
	Player->RunCamShake(EPlayerCamShake::Execute);
	D("Weapon Melee SubAction invoked", 1.0f);
}

bool AWeaponMeleeBase::CanAttack()
{
	return (Player->SwingCount < MaxAttackNumber && !Player->bDoingAttack);
}

void AWeaponMeleeBase::PerformTrace()
{
	bool bResult;
	FHitResult HitResult;
	FVector ColStart = WeaponMesh->GetSocketLocation("meleecolstart");
	FRotator ColRotation = WeaponMesh->GetSocketRotation("meleecolstart");
	FVector ColEnd = WeaponMesh->GetSocketLocation("meleecolend");

	bResult = UKismetSystemLibrary::BoxTraceSingle(WeaponMesh, ColStart, ColEnd, FVector(10.f, 10.f, 10.f), ColRotation,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), false, {}, EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.0f);

	if (bResult)
	{
		class AZombieBase* HitZombie = Cast<AZombieBase>(HitResult.GetActor());
		if (!HitZombie) return;

		if (Player->OnWeaponHit.IsBound()) Player->OnWeaponHit.Broadcast();

		if(AttackSound) AttackSound->Play();
		
		Player->RunCamShake(EPlayerCamShake::BatAttack);
		UGameplayStatics::ApplyDamage(HitZombie, Damage, Player->GetController(), Player, nullptr);
		bCollisionOn = false;
	}
}

void AWeaponMeleeBase::ConvertPlayerController()
{
	// TODO: Implement this if melee noise is needed
	return;
}

void AWeaponMeleeBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)	return;

	if (Player)
	{
		Player->bCannotAction = false;

		Player->SwingCount = 0;
		Player->PlayerAnimInstance->AttackEndDelegate.Unbind();
	}
}


