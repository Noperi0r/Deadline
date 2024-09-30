// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieBaseController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API AZombieBaseController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = AI)
	float Aggro;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Threshold;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TurnThreshold;

	UFUNCTION(BlueprintCallable)
	void SetTargetPlayer();
protected:


	UFUNCTION(BlueprintCallable)
	void HandleNoise(float Noise);

	UFUNCTION(BlueprintCallable)
	void HandleSight(float count);

	UFUNCTION(BlueprintNativeEvent)
	void CheckThreshold();
	virtual void CheckThreshold_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Turn();
	void Turn_Implementation();

};
