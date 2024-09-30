#pragma once

#include "CoreMinimal.h"
#include "FBgmSoundData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FBgmSoundData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FBgmSoundData()
	: Id(0)
	, SoundWave(nullptr)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BGM, meta = (ExposeOnSpawn=true))
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BGM, meta = (ExposeOnSpawn=true))
	TSoftObjectPtr<USoundWave> SoundWave;
};
