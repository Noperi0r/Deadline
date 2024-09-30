// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify\AnimNotifyState_CannotDoAttack.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_CannotDoAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (Player)
	{
		Player->bDoingAttack = true;
	}
}

void UAnimNotifyState_CannotDoAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Player)
	{
		Player->bDoingAttack = false;
	}
}
