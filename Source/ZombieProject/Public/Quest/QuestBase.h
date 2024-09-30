#pragma once

#include "CoreMinimal.h"
#include "EQuestStatus.h"
#include "FQuestData.h"
#include "SubTask/SubTaskBase.h"
#include "UObject/Object.h"
#include "QuestBase.generated.h"

// Event delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDelegateSignature, FQuestData, QuestData);

/**
 *
 */
UCLASS(Blueprintable)
class ZOMBIEPROJECT_API UQuestBase : public UObject
{
    GENERATED_BODY()
public:
    UQuestBase();

    virtual void BeginPlay();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnBeginPlay();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnEndPlay();

#pragma region "Event Functions"
    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnQuestStart();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnSubTaskStart();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnSubTaskEnd();

    UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
    void OnQuestEnd();
#pragma endregion

#pragma region "Public Function"
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest")
    bool CanAcceptQuest() const;

    bool CanAcceptQuest_Implementation() const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Quest")
    bool CanComplete() const;

    bool CanComplete_Implementation() const;
#pragma endregion

#pragma region "Event Delegates"
protected:
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FQuestEventDelegateSignature OnQuestStartDelegate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FSubTaskEventDelegateSignature OnSubTaskStartDelegate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FSubTaskEventDelegateSignature OnSubTaskEndDelegate;

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Quest")
    FQuestEventDelegateSignature OnQuestEndDelegate;
#pragma endregion

#pragma region "Properties"
    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    FString QuestName;

    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    FGuid QuestId;

    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    FString QuestDescription;

    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    EQuestStatus CurrentStatus;

    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    TArray<USubTaskBase*> SubTasks;

    UPROPERTY(BlueprintReadWrite, Category = "Quest", meta = (ExposeOnSpawn=true))
    int32 CurrentTaskIndex;

    UPROPERTY(BlueprintReadWrite, Category = "Quest")
    USubTaskBase* CurrentTask;

    UPROPERTY(BlueprintReadWrite, Category = "Objective", meta = (ExposeOnSpawn=true))
    bool IsComplete;

    static constexpr int Max_Sub_Tasks = 5;

    int SubTaskCount = 0;
#pragma endregion

#pragma region "Getter and Setters"
public:
    UFUNCTION(BlueprintCallable, Category = "Quest")
    FGuid GetQuestId() const { return QuestId; }

    UFUNCTION(BlueprintCallable, Category = "Quest")
    void SetQuestId(FGuid NewQuestId) { QuestId = NewQuestId; }

    UFUNCTION(BlueprintCallable, Category = "Quest")
    FString GetQuestName() const { return QuestName; }

    UFUNCTION(BlueprintCallable, Category = "Quest")
    void SetQuestName(const FString& NewQuestName) { QuestName = NewQuestName; }

    UFUNCTION(BlueprintCallable, Category = "Quest")
    USubTaskBase* GetCurrentTask() const;

    UFUNCTION(BlueprintCallable, Category = "Quest")
    void SetCurrentTask(USubTaskBase* NewTask);

    UFUNCTION(BlueprintCallable, Category = "Quest")
    EQuestStatus GetCurrentStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Quest")
    void SetCurrentStatus(EQuestStatus NewStatus);
#pragma endregion

    UFUNCTION(BlueprintCallable, Category = "Quest")
    FQuestData GetQuestData() const;
};