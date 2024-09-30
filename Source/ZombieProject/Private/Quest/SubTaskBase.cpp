// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/SubTask/SubTaskBase.h"

USubTaskBase::USubTaskBase(): TaskType(ESubTaskType::Interact),
                              TaskStatus(ETaskStatus::NotStarted),
                              TaskCount(0),
                              TaskGoal(0),
                              IsComplete(false)
{
}

bool USubTaskBase::CanStartTask_Implementation()
{
	return true;
}

bool USubTaskBase::CanCompleteTask_Implementation()
{
	return true;
}

void USubTaskBase::StartTask_Implementation()
{
	OnSubTaskStartDelegate.Broadcast(GetTaskData());
}

void USubTaskBase::CompleteTask_Implementation()
{
	OnSubTaskEndDelegate.Broadcast(GetTaskData());
}

