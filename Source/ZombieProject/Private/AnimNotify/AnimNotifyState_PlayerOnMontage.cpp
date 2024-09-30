// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotify\AnimNotifyState_PlayerOnMontage.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_PlayerOnMontage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (Player)
	{
	}
}

void UAnimNotifyState_PlayerOnMontage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
