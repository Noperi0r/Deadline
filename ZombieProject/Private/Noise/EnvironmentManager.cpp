// Fill out your copyright notice in the Description page of Project Settings.


#include "Noise/EnvironmentManager.h"

#include "Kismet/GameplayStatics.h"

void UEnvironmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnvironmentVolume::StaticClass(), FoundActors);

	EnvironmentVolumes.Empty(FoundActors.Num());
	for (auto Actor : FoundActors)
	{
		auto Volume = Cast<AEnvironmentVolume>(Actor);
		if (Volume)
		{
			EnvironmentVolumes.Add(Volume);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Environment Volumes: %d"), EnvironmentVolumes.Num());
}

void UEnvironmentManager::Deinitialize()
{
	Super::Deinitialize();
}

EEnvironmentType UEnvironmentManager::GetCurrentEnvironment(const FVector3d& Position) const
{
	for (auto Element : EnvironmentVolumes)
	{
		if (Element.IsValid())
		{
			if (Element->CheckPositionInVolume(Position))
			{
				return Element->GetEnvironmentType();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Environment Volume"));
		}
	}

	return EEnvironmentType::TE_Plains;
}

float UEnvironmentManager::GetEnvironmentNoiseRangeMultiplier(EEnvironmentType EnvironmentType)
{
	switch (EnvironmentType)
	{
		case EEnvironmentType::TE_Plains:
			return 1.0f;
		case EEnvironmentType::TE_Room:
			return 1.5f;
		case EEnvironmentType::TE_Plaza:
			return 1.3f;
		case EEnvironmentType::TE_Sewer:
			return 2.0f;
		default:
			return 1.0f;
	}
}

float UEnvironmentManager::GetEnvironmentNoiseRangeMultiplier(const FVector3d& Position) const
{
	auto env = GetCurrentEnvironment(Position);
	return GetEnvironmentNoiseRangeMultiplier(env);
}
