// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NoiseEmitterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNoiseEmitterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIEPROJECT_API INoiseEmitterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ZombieProjectNoiseEmitter)
	void MakeEnvironmentNoise(APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ZombieProjectNoiseEmitter)
	void MakeContinuousEnvironmentNoise(APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range, float Duration);
};
