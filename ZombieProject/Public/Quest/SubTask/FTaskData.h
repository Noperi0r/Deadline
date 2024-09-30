#pragma once

#include "ESubTaskType.h"
#include "ETaskStatus.h"
#include "Quest/FObjectiveData.h"
#include "FTaskData.generated.h"

USTRUCT(BlueprintType)
struct FTaskData
{
	GENERATED_USTRUCT_BODY()

#pragma region "Constructors"
	FTaskData() : TaskName(""),
	              TaskId(FGuid()),
	              TaskType(ESubTaskType::Kill),
	              TaskDescription(""),
	              TaskStatus(ETaskStatus::NotStarted),
	              TaskObjectiveDataList(TArray<FObjectiveData>()),
	              TaskCount(0),
	              TaskGoal(0),
	              IsComplete(false),
	              GuideLocation(FVector::ZeroVector)
	{
	}

	FTaskData(const FString& TaskName, const FGuid& TaskId, const ESubTaskType TaskType, const FString& TaskDescription,
	          const ETaskStatus TaskStatus, const TArray<FObjectiveData>& TaskObjectiveDataList, const int32 TaskCount,
	          const int32 TaskGoal, const bool bIsComplete, const FVector& GuideLocation)
		: TaskName(TaskName),
		  TaskId(TaskId),
		  TaskType(TaskType),
		  TaskDescription(TaskDescription),
		  TaskStatus(TaskStatus),
		  TaskObjectiveDataList(TaskObjectiveDataList),
		  TaskCount(TaskCount),
		  TaskGoal(TaskGoal),
		  IsComplete(bIsComplete),
		  GuideLocation(GuideLocation)
	{
	}
#pragma endregion

#pragma region "Properties"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	FString TaskName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	FGuid TaskId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	ESubTaskType TaskType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	FString TaskDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	ETaskStatus TaskStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	TArray<FObjectiveData> TaskObjectiveDataList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	int32 TaskCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	int32 TaskGoal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
	bool IsComplete;
	
	// Guide location (optional)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true), meta = (EditCondition = "bHasGuideLocation"))
	FVector GuideLocation;
#pragma endregion
};