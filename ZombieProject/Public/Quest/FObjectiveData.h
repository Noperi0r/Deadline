#pragma once
#include "EObjectiveType.h"

#include "FObjectiveData.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveData
{
	GENERATED_BODY()

	FObjectiveData() : ObjectiveName(""),
	                   ObjectiveType(EObjectiveType::Interact),
	                   IsComplete(false),
					   ObjectiveDescription("")
	{
	}
	
	FObjectiveData(const FString& ObjectiveName, const EObjectiveType ObjectiveType, const bool bIsComplete, const FString& ObjectiveDescription)
		: ObjectiveName(ObjectiveName),
		  ObjectiveType(ObjectiveType),
		  IsComplete(bIsComplete),
		  ObjectiveDescription(ObjectiveDescription)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
	FString ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
	bool IsComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
	FString ObjectiveDescription;
};
