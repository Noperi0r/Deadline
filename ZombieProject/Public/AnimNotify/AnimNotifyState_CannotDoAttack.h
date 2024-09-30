// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CannotDoAttack.generated.h"

/**
 * The reason why this notify class is used is because montage delegate cannot handle situations where variable value should change in the middle of montage, not the end.
 */
UCLASS()
class ZOMBIEPROJECT_API UAnimNotifyState_CannotDoAttack : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	class AZombiePlayer* Player;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
