// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearNoiseLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearNoiseLocation::UBTTask_ClearNoiseLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Clear Noise Location");
}

EBTNodeResult::Type UBTTask_ClearNoiseLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	MyBlackboard->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}
