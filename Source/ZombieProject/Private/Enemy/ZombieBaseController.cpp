// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ZombieBaseController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AZombieBaseController::SetTargetPlayer()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

	if(BlackboardComp)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		BlackboardComp->SetValueAsObject("TargetActor", PlayerPawn);
	}
}

void AZombieBaseController::HandleNoise(float Noise)
{
	Aggro += Noise;
	CheckThreshold();
	
}

void AZombieBaseController::HandleSight(float count)
{
	Aggro += count;
	CheckThreshold();
}

void AZombieBaseController::CheckThreshold_Implementation()
{
	if(Aggro >= TurnThreshold && Aggro < Threshold)
	{
		Turn();
	}
	else if(Aggro >= Threshold)
	{
		SetTargetPlayer();
	}

}

void AZombieBaseController::Turn_Implementation()
{
	Turn();
}
