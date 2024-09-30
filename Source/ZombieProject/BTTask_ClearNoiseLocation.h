// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearNoiseLocation.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UBTTask_ClearNoiseLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

private:

public:
	UBTTask_ClearNoiseLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
