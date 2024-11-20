// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotifyState_EndExecute.h"

#include "Enemy/ZombieBase.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_EndExecute::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	Player->bOnExecute = false;
}

void UAnimNotifyState_EndExecute::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if(!Player) return;

	
}

void UAnimNotifyState_EndExecute::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(!Player) return;
}
