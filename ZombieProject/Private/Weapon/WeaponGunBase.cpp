// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon\WeaponGunBase.h"
#include "Kismet\GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "Enemy\ZombieBase.h"
#include "CameraShakeFire.h"
#include "CameraShakeWalk.h"
#include "Player\ZombiePlayerController.h"
#include "Components\AudioComponent.h"

AWeaponGunBase::AWeaponGunBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Weapon Mesh"));
	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	WeaponMesh->SetOnlyOwnerSee(false);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	// Set ItemTag
	ItemTag = EWeaponTag::RANGE;
	//FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire Effect"));

	Montage_Reload = CreateDefaultSubobject<UAnimMontage>(TEXT("Reload Montage"));
	Montage_AimAttack = CreateDefaultSubobject<UAnimMontage>(TEXT("Aim Fire Montage"));

	CurrentBullet = 1;
	MaxBullet = 1;

	Damage = 100.0f;

	WeaponLoudness = 100.0f;
	WeaponLoudnessRange = 4000.0f;

	AttackSound->SetupAttachment(WeaponMesh, FName("S_SoundLocation"));
	
	ReloadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Reload Sound"));
	ReloadSound->bAutoActivate = false;
	ReloadSound->SetupAttachment(WeaponMesh, FName("S_ReloadSound"));
}

void AWeaponGunBase::BeginPlay()
{
	Super::BeginPlay();
}

bool AWeaponGunBase::CanAttack() // In Player or in Weapon? > �ּ� �÷��̾ bCanShoot �����
{
	return (CurrentBullet > 0);
}

void AWeaponGunBase::PerformTrace()
{
	FHitResult HitResult;
	FVector Start = MuzzleLocation->GetComponentLocation();
	FVector End = Start + MuzzleLocation->GetForwardVector() * 3000.f; // 500 unit means 500 cm long line
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bResult = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params, FCollisionResponseParams());
	if (bResult)
	{

		AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor());
		if (!Zombie) return;

		if (Player->OnWeaponHit.IsBound()) Player->OnWeaponHit.Broadcast();
		UGameplayStatics::ApplyDamage(Zombie, Damage, Player->GetController(), Player, nullptr);
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, .5f, 0, 3.f);
}

void AWeaponGunBase::ConvertPlayerController()
{
	check(Player);
	Controller = Cast<AZombiePlayerController>(Player->GetController());
}

void AWeaponGunBase::MakeWeaponNoise(const FVector& Location)
{
}

void AWeaponGunBase::Attack()
{
	if (Player->bIsAiming)
		Player->PlayerAnimInstance->Montage_Play(Montage_AimAttack);
	else
		Player->PlayerAnimInstance->Montage_Play(Montage_Attack);

	PerformTrace();

	Controller->OnNoiseDelegate.Broadcast(WeaponLoudness, GetActorLocation(), WeaponLoudnessRange);

	Player->RunCamShake(EPlayerCamShake::RifleAttack);

	if (Player->OnGunFire.IsBound()) Player->OnGunFire.Broadcast();
	if (AttackSound) AttackSound->Play();

	if (--CurrentBullet <= 0)
		Reload();
}

void AWeaponGunBase::SubAction()
{
	if (!Player->bCannotAction)
	{
		if (!Player->bIsAiming)
		{
			Player->bIsAiming = true;
		}
		else
		{
			Player->bIsAiming = false;
		}
	}
}

void AWeaponGunBase::Reload() 
{
	if (CurrentBullet == MaxBullet) return;
	
	Player->bCannotAction = true;

	Player->PlayerAnimInstance->Montage_Play(Montage_Reload);
	Player->PlayerAnimInstance->WeaponActionEndDelegate.BindUFunction(this, FName("OnReloadProcessEnd"));
	Player->PlayerAnimInstance->Montage_SetEndDelegate(Player->PlayerAnimInstance->WeaponActionEndDelegate, Montage_Reload);
	//Player->bIsAiming = false;
}


void AWeaponGunBase::FailFire()
{
}


void AWeaponGunBase::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	return;
}

void AWeaponGunBase::OnReloadProcessEnd(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentBullet = MaxBullet;
	Player->bCannotAction = false;
	Player->PlayerAnimInstance->WeaponActionEndDelegate.Unbind();
}

//void AWeaponGunBase::OnReloadProcessEnd_NotAim(UAnimMontage* Montage, bool bInterrupted)
//{
//	// TODO: Load bullets in Inventory and get maxbullet
//	//Player->bIsAiming = false;
//
//	CurrentBullet = MaxBullet;
//	Player->bCannotAction = false;
//	Player->PlayerAnimInstance->WeaponActionEndDelegate.Unbind();
//}
//
//void AWeaponGunBase::OnReloadProcessEnd_Aim(UAnimMontage* Montage, bool bInterrupted)
//{
//	// TODO: Load bullets in Inventory and get maxbullet
//	//Player->bIsAiming = true;
//
//	CurrentBullet = MaxBullet;
//	Player->bCannotAction = false;
//	Player->PlayerAnimInstance->WeaponActionEndDelegate.Unbind();
//}

