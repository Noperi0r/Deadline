// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_cppHunterCheckPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UBtsCppHunterCheckPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
	
	
	public:
		FBlackboardKeySelector blackboardKey;
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
		
};
