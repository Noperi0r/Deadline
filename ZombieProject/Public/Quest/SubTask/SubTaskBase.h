#pragma once

#include "CoreMinimal.h"
#include "ESubTaskType.h"
#include "ETaskStatus.h"
#include "FTaskData.h"
#include "UObject/Object.h"
#include "Quest/FObjectiveData.h"
#include "SubTaskBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSubTaskEventDelegateSignature, FTaskData, SubTaskData);

UCLASS(Blueprintable)
class ZOMBIEPROJECT_API USubTaskBase : public UObject
{
    GENERATED_BODY()

public:
    USubTaskBase();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnBeginPlay();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnEndPlay();

#pragma region "Getter and Setter"
    // Getter and Setter for Task ID
    UFUNCTION(BlueprintCallable, Category = "Task")
    FGuid GetTaskId() const { return TaskId; }

    UFUNCTION(BlueprintCallable, Category = "Task")
    void SetTaskId(FGuid NewTaskId) { TaskId = NewTaskId; }

    // Getter and Setter for Task status
    UFUNCTION(BlueprintCallable, Category = "Task")
    ETaskStatus GetStatus() const { return TaskStatus; }

    UFUNCTION(BlueprintCallable, Category = "Task")
    void SetStatus(ETaskStatus NewStatus) { TaskStatus = NewStatus; }

    // Getter and Setter for Guide Location
    UFUNCTION(BlueprintCallable, Category = "Task")
    FVector GetGuideLocation() const { return GuideLocation; }

    UFUNCTION(BlueprintCallable, Category = "Task")
    void SetGuideLocation(const FVector NewGuideLocation) { GuideLocation = NewGuideLocation; }
#pragma endregion

#pragma region "Properties"
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    FString TaskName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    FGuid TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    ESubTaskType TaskType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    FString TaskDescription;

    UPROPERTY(BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    ETaskStatus TaskStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    TArray<FObjectiveData> TaskObjectiveNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    int32 TaskCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true))
    int32 TaskGoal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
    bool IsComplete;

    // Guide location (optional)
    UPROPERTY(BlueprintReadWrite, Category = "Task", meta = (ExposeOnSpawn=true), meta = (EditCondition = "bHasGuideLocation"))
    FVector GuideLocation;
#pragma endregion

#pragma region "Delegates"
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FSubTaskEventDelegateSignature OnSubTaskStartDelegate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FSubTaskEventDelegateSignature OnSubTaskEndDelegate;
#pragma endregion

#pragma region "Task Functions"
public:
    // Can start task delegate
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
    bool CanStartTask();

    // Can complete task delegate
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
    bool CanCompleteTask();

    virtual bool CanStartTask_Implementation();

    virtual bool CanCompleteTask_Implementation();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
    void StartTask();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Task")
    void CompleteTask();

    virtual void StartTask_Implementation();

    virtual void CompleteTask_Implementation();
#pragma endregion

    FTaskData GetTaskData() const
    {
        return FTaskData(
            TaskName,
            TaskId,
            TaskType,
            TaskDescription,
            TaskStatus,
            TaskObjectiveNames,
            TaskCount,
            TaskGoal,
            IsComplete,
            GuideLocation
        );
    }
};