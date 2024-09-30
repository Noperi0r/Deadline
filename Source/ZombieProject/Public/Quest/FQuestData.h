#pragma once

#include <Engine/DataTable.h>

#include "EQuestStatus.h"
#include "SubTask/FTaskData.h"
#include "FQuestData.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FQuestData() : QuestName(""),
	               QuestId(FGuid()),
	               QuestDescription(""),
	               QuestStatus(EQuestStatus::NotStarted),
	               SubTasks(TArray<FTaskData>()),
	               CurrentTaskIndex(0),
	               IsComplete(false)
	{
	}

	FQuestData(const FString& QuestName, const FGuid& QuestId, const FString& QuestDescription,
	           const EQuestStatus QuestStatus, const TArray<FTaskData>& SubTasks, const int32 CurrentTaskIndex,
	           const bool bIsComplete)
		: QuestName(QuestName),
		  QuestId(QuestId),
		  QuestDescription(QuestDescription),
		  QuestStatus(QuestStatus),
		  SubTasks(SubTasks),
		  CurrentTaskIndex(CurrentTaskIndex),
		  IsComplete(bIsComplete)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	FGuid QuestId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	EQuestStatus QuestStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	TArray<FTaskData> SubTasks;

	// Current task in the quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	int32 CurrentTaskIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
	bool IsComplete;
};
