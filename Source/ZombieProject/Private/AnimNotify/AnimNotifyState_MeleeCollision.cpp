// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify\AnimNotifyState_MeleeCollision.h"
#include "Player\ZombiePlayer.h"
#include "Weapon\WeaponMeleeBase.h"
#include "Kismet\KismetSystemLibrary.h"

void UAnimNotifyState_MeleeCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (Player)
	{
		if (AWeaponMeleeBase* Weapon = Cast<AWeaponMeleeBase>(Player->CurrentWeapon))
		{
			Weapon->bCollisionOn = true;
		}
	}

}

void UAnimNotifyState_MeleeCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (Player)
	{
		if (AWeaponMeleeBase* Weapon = Cast<AWeaponMeleeBase>(Player->CurrentWeapon))
		{
			Weapon->bCollisionOn = false;
		}
	}
}
