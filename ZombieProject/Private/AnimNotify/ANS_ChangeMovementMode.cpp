// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\ANS_ChangeMovementMode.h"
#include "Player\ZombiePlayer.h"

void UANS_ChangeMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;

	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}
