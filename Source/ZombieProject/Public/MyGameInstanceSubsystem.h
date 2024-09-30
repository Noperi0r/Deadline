// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Item/PickableActor.h"

#include "MyGameInstanceSubsystem.generated.h"

/**
 * 
 */
struct FPlayerDataExample
{
	FName PlayerName;
	int32 PlayerLevel;
	int32 LevelProgress;
	float PlayerHealth;
	FTransform PlayerTransform;
	//etc...
};


UCLASS()
class ZOMBIEPROJECT_API UMyGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//End USubsystem Interface

	void SaveGame();
	void SavePlayer();
	void SavePickableActors();

	void UpdateActorData(FName Name, FPickableObjectData ActorData);

	void LoadGame();
	void LoadPlayer();
	void LoadPickableActors();

private:
	FPlayerDataExample PlayerData;
	//FPickableObjectData PickableActorsData;
	TArray<FPickableObjectData> _changeList;

	//UMySaveGame* LevelSaveFile;
	//USaveGame* PlayerSaveFile;
};
