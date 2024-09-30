// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotifyState_Death.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_Death::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
}

void UAnimNotifyState_Death::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Player->bOnDead = false;
}
