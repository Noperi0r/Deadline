#pragma once
#include "CoreMinimal.h"
#include "ZombieData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FZombieData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAlive;
};
