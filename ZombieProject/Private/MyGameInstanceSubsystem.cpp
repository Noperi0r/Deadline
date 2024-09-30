// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstanceSubsystem.h"


void UMyGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMyGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMyGameInstanceSubsystem::SaveGame()
{
	// if Player LevelSaveFile is not exist
	// Only called when first time in new game or Save file is broken
	{
		// make new Player LevelSaveFile
		//UELog("Player LevelSaveFile is not exist");
		//UPlayerFile* PlayerSaveFile = ...;
		//UELog("Making Player LevelSaveFile is Completed");

	}
	// Save Player
	SavePlayer();

	// if LevelSaveFile of Current Level is not exist 
	{
		// make new LevelSaveFile
		//UELog("Level LevelSaveFile is not exist");
		//UMySaveFile* LevelSaveFile = ...;
		//UELog("Making LevelSaveFile is Completed");
	}
	SavePickableActors();
}

void UMyGameInstanceSubsystem::SavePlayer()
{
	// Save All Player Data to PlayerSaveFile
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//Cast<MyPlayerController>(PlayerController)->SaveGame();
}

void UMyGameInstanceSubsystem::SavePickableActors()
{
	//Save Actors()
	//TArray<AActor*> PickableActors = TArray<AActor*>();
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickableActor::StaticClass(), PickableActors);
	//for (auto Actor : PickableActors)
	//{
	//	//Cast<APickableActor>(Actor)->SaveGame(); // 인터페이스로 변경하는게 나을듯
	//}

	///// USE THIS
	// Save Only Changed PickableActors to LevelSaveFile
	//for(auto Name, ActorData in  _changeList)
	//{
	//	LevelSaveFile->SaveActor(Name, Data->GetIsEnabled(), Data);	
	//}
	// And Flush buffer >> _changeList.flush();

}

void UMyGameInstanceSubsystem::UpdateActorData(FName Name, FPickableObjectData ActorData)
{
	//if(Name in _changeList)
	//{
	//	_changeList[Name] = ActorData;
	//}
	//else
	//{
	//	_changeList.append({ Name, ActorData });
	//}
}

void UMyGameInstanceSubsystem::LoadGame()
{
	// if LevelSaveFile of Current Level is not exist 
	{
		// Level is Initialized as Default
	}
	//else
	{
		//Load Actors
		LoadPickableActors();
	}
}

void UMyGameInstanceSubsystem::LoadPlayer()
{
	// LoadPlayer
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//Get Player Data from PlayerSaveFile
	//FPlayerDataExample PlayerData = PlayerSaveFile->GetPlayerData();
	//Cast<MyPlayerController>(PlayerController)->LoadGame(PlayerData);
}

void UMyGameInstanceSubsystem::LoadPickableActors()
{
	//LoadActors()
	TArray<AActor*> PickableActors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickableActor::StaticClass(), PickableActors);
	for (auto Actor : PickableActors)
	{
		//FPickableObjectData ActorData = LevelSaveFile->GetActorData(Actor->GetName());
		//Cast<APickableActor>(Actor)->LoadGame(ActorData); // 인터페이스로 변경하는게 나을듯
	}
}
