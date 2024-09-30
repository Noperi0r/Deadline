// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentVolume.h"
#include "EEnvironmentType.h"

#include "GameFramework/Actor.h"
#include "EnvironmentManager.generated.h"

UCLASS()
class UEnvironmentManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// USubsystem Ω√¿€
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem ≥°
	
	float GetEnvironmentNoiseRangeMultiplier(const FVector3d& Position) const;

private:
	TArray<TWeakObjectPtr<AEnvironmentVolume>> EnvironmentVolumes;

	EEnvironmentType GetCurrentEnvironment(const FVector3d& Position) const;

	static float GetEnvironmentNoiseRangeMultiplier(EEnvironmentType EnvironmentType);

};
