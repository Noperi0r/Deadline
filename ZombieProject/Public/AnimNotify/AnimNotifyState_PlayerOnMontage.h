// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PlayerOnMontage.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UAnimNotifyState_PlayerOnMontage : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	class AZombiePlayer* Player;

private:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
