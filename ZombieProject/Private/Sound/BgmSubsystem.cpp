// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/BgmSubsystem.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/FBgmSoundData.h"

/**
 * @brief Constructor for the UBgmSubsystem class.
 *
 * This constructor initializes the CurrentBgm variable to nullptr and loads the BgmDataTable.
 */
UBgmSubsystem::UBgmSubsystem(): CurrentBgm(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BgmDataTableObject(
		TEXT("/Script/Engine.DataTable'/Game/ZombieProject/Sound/DT_BgmTable.DT_BgmTable'"));
	if (BgmDataTableObject.Succeeded())
	{
		BgmDataTable = BgmDataTableObject.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load data table."));
	}
}

/**
 * @brief Initializes the BgmSubsystem.
 *
 * This method is called to initialize the BgmSubsystem. It overrides the Initialize method from the
 * UGameInstanceSubsystem class.
 *
 * @param Collection The subsystem collection to which the BgmSubsystem belongs.
 *
 * @note This method must be called before using any other functionality of the BgmSubsystem.
 *
 * @see UBgmSubsystem
 * @see UGameInstanceSubsystem
 */
void UBgmSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

/**
 * @brief Deinitializes the BgmSubsystem.
 *
 * This function is called when the BgmSubsystem is being deinitialized.
 * It calls the parent class's Deinitialize function.
 *
 * @see UGameInstanceSubsystem::Deinitialize()
 */
void UBgmSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

/**
 * @brief Plays the background music (BGM) with the given ID.
 *
 * @param BgmId The ID of the BGM to be played.
 */
void UBgmSubsystem::PlayBgm(const int BgmId)
{
    auto bgm = BgmDataTable->FindRow<FBgmSoundData>(FName(*FString::FromInt(BgmId)), TEXT("Id"), true);
    
    if(bgm == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find BGM data."));
        return;
    }
    
    if(bgm->SoundWave.IsValid())
    {
        USoundWave* sound = bgm->SoundWave.Get();
        UGameplayStatics::PlaySound2D(GetWorld(), sound);
    }
    else
    {
        // Load SoundWave Asynchronously
        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
        Streamable.RequestAsyncLoad(bgm->SoundWave.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &UBgmSubsystem::OnBgmLoaded, BgmId));
    }
    CurrentBgm = bgm->SoundWave;
}

/**
 * \brief Handles the callback when a background music (BGM) is loaded.
 *
 * \param BgmId The ID of the BGM to be loaded.
 *
 * This method is called asynchronously when a BGM is loaded. It retrieves the BGM data from the BgmDataTable based on the provided ID.
 * If the BGM data is found and the associated SoundWave is valid, the BGM is played in 2D using UGameplayStatics::PlaySound2D.
 * If the BGM data is not found or the SoundWave is invalid, an error message is logged.
 */
void UBgmSubsystem::OnBgmLoaded(const int BgmId) const
{
    auto bgm = BgmDataTable->FindRow<FBgmSoundData>(FName(*FString::FromInt(BgmId)), TEXT("Id"), true);
    
    if(bgm && bgm->SoundWave.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("BGM loaded successfully."));
        UGameplayStatics::PlaySound2D(GetWorld(), bgm->SoundWave.Get());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load BGM."));
    }
}
