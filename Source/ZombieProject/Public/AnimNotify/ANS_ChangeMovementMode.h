// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ChangeMovementMode.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UANS_ChangeMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	class AZombiePlayer* Player;
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
};
