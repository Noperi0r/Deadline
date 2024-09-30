// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/QuestBase.h"

UQuestBase::UQuestBase(): CurrentStatus(EQuestStatus::NotStarted),
                          CurrentTaskIndex(0),
                          CurrentTask(nullptr),
                          IsComplete(false)
{
	if(CurrentTaskIndex < SubTasks.Num())
	{
		CurrentTask = SubTasks[CurrentTaskIndex];
	}
}

void UQuestBase::BeginPlay()
{
}

USubTaskBase* UQuestBase::GetCurrentTask() const
{
	return (CurrentTaskIndex >= SubTasks.Num()) ? SubTasks.Last() : SubTasks[CurrentTaskIndex];
}

void UQuestBase::SetCurrentTask(USubTaskBase* NewTask)
{
	CurrentTask = NewTask;
}

EQuestStatus UQuestBase::GetCurrentStatus() const
{
	return CurrentStatus;
}

void UQuestBase::SetCurrentStatus(EQuestStatus NewStatus)
{
	CurrentStatus = NewStatus;
}

FQuestData UQuestBase::GetQuestData() const
{
	auto SubTaskDatas = TArray<FTaskData>();
	for(auto SubTask : SubTasks)
	{
		SubTaskDatas.Add(SubTask->GetTaskData());
	}
	return FQuestData(QuestName, QuestId, QuestDescription, CurrentStatus, SubTaskDatas, CurrentTaskIndex, IsComplete);
}

bool UQuestBase::CanAcceptQuest_Implementation() const
{
	return true;
}

bool UQuestBase::CanComplete_Implementation() const
{
	return true;
}