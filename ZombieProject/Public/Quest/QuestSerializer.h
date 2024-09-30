// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "FQuestData.h"
#include "SubTask/FTaskData.h"
#include "FObjectiveData.h"

#include "QuestSerializer.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UQuestSerializer : public UObject
{
	GENERATED_BODY()

public:
#pragma region "Serialization"
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "QuestDataToDataTable"), Category = "Quest")
	static FString QuestToDataTable(FQuestData QuestData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SubTaskToDataTable"), Category = "Quest")
	static FString SubTaskToDataTable(FTaskData TaskData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ObjectiveToDataTable"), Category = "Quest")
	static FString ObjectiveToDataTable(FObjectiveData ObjectiveData);
#pragma endregion
};
