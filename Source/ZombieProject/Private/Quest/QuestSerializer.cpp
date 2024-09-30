// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest/QuestSerializer.h"

FString UQuestSerializer::QuestToDataTable(FQuestData QuestData)
{
	auto args = FStringFormatNamedArguments();
	const TEnumAsByte QuestStatusEnum = QuestData.QuestStatus;
	FString QuestStatusEnumAsString = UEnum::GetValueAsString(QuestStatusEnum.GetValue());
	const FString QuestStatusEnumName = TEXT("EQuestStatus::");
	QuestStatusEnumAsString.RemoveFromStart(QuestStatusEnumName);

	auto SubTaskList = QuestData.SubTasks;
	TArray<FString> SubTaskListString;
	for (auto subTask : SubTaskList)
	{
		SubTaskListString.Add(SubTaskToDataTable(subTask));
	}
	FString SubTaskListAsString = FString::Join(SubTaskListString, TEXT(","));

	args.Add(TEXT("QuestName"), QuestData.QuestName);
	args.Add(TEXT("QuestId"), QuestData.QuestId.ToString());
	args.Add(TEXT("QuestDescription"), QuestData.QuestDescription);
	args.Add(TEXT("QuestStatus"), QuestStatusEnumAsString);
	args.Add(TEXT("SubTasks"), SubTaskListAsString);
	args.Add(TEXT("CurrentTaskIndex"), QuestData.CurrentTaskIndex);
	args.Add(TEXT("IsComplete"), QuestData.IsComplete? TEXT("true") : TEXT("false"));
	
	const wchar_t* InFormatString = L"\"{QuestName}\",\"{QuestId}\",\"{QuestDescription}\",\"{QuestStatus}\",\"({SubTasks})\",{CurrentTaskIndex},\"{IsComplete}\"";
	auto SerializedQuestData = FString::Format(InFormatString, args);
	UE_LOG(LogTemp, Log, TEXT("SerializedQuestData: %s"), *SerializedQuestData);
	return SerializedQuestData;
}

FString UQuestSerializer::SubTaskToDataTable(FTaskData TaskData)
{
	auto args = FStringFormatOrderedArguments();
	const TEnumAsByte TaskTypeEnum = TaskData.TaskType;
	FString EnumAsString = UEnum::GetValueAsString(TaskTypeEnum.GetValue());
	const FString EnumName = TEXT("ESubTaskType::");
	EnumAsString.RemoveFromStart(EnumName);

	const TEnumAsByte TaskStatusEnum = TaskData.TaskStatus;
	FString TaskStatusEnumAsString = UEnum::GetValueAsString(TaskStatusEnum.GetValue());
	const FString TaskStatusEnumName = TEXT("ETaskStatus::");
	TaskStatusEnumAsString.RemoveFromStart(TaskStatusEnumName);

	auto ObjectiveDataList = TaskData.TaskObjectiveDataList;
	TArray<FString> ObjectiveDataListString;
	for (auto objectiveData : ObjectiveDataList)
	{
		ObjectiveDataListString.Add(ObjectiveToDataTable(objectiveData));
	}
	FString ObjectiveDataListAsString = FString::Join(ObjectiveDataListString, TEXT(","));

	auto guideLocationString = TaskData.GuideLocation.ToString();
	guideLocationString.ReplaceCharInline(*" ", *",");
	
	args.Add(TaskData.TaskName);
	args.Add(TaskData.TaskId.ToString());
	args.Add(EnumAsString);
	args.Add(TaskData.TaskDescription);
	args.Add(TaskStatusEnumAsString);
	args.Add(ObjectiveDataListAsString);
	args.Add(TaskData.TaskCount);
	args.Add(TaskData.TaskGoal);
	args.Add(TaskData.IsComplete? TEXT("true") : TEXT("false"));
	args.Add(guideLocationString);
	
	const wchar_t* InFormatString = L"(TaskName=\"\"{0}\"\",TaskId={1},TaskType={2},TaskDescription=\"\"{3}\"\",TaskStatus={4},TaskObjectiveDataList=({5}),TaskCount={6},TaskGoal={7},IsComplete={8},GuideLocation=({9}))";
	auto SerializedTaskData = FString::Format(InFormatString, args);
	UE_LOG(LogTemp, Log, TEXT("SerializedTaskData: %s"), *SerializedTaskData);
	return SerializedTaskData;
}

FString UQuestSerializer::ObjectiveToDataTable(FObjectiveData ObjectiveData)
{
	auto args = FStringFormatNamedArguments();
	const TEnumAsByte ObjectiveTypeEnum = ObjectiveData.ObjectiveType;
	FString EnumAsString = UEnum::GetValueAsString(ObjectiveTypeEnum.GetValue());
	const FString EnumName = TEXT("EObjectiveType::");
	EnumAsString.RemoveFromStart(EnumName);

	args.Add(TEXT("ObjectiveName"), ObjectiveData.ObjectiveName);
	args.Add(TEXT("ObjectiveType"), EnumAsString);
	args.Add(TEXT("IsComplete"), ObjectiveData.IsComplete? TEXT("true") : TEXT("false"));
	args.Add(TEXT("ObjectiveDescription"), ObjectiveData.ObjectiveDescription);

	const wchar_t* InFormatString = L"(ObjectiveName=\"\"{ObjectiveName}\"\",ObjectiveType={ObjectiveType},IsComplete={IsComplete},ObjectiveDescription=\"\"{ObjectiveDescription}\"\")";
	auto SerializedObjectiveData = FString::Format(InFormatString, args);
	UE_LOG(LogTemp, Log, TEXT("SerializedObjectiveData: %s"), *SerializedObjectiveData);
	return SerializedObjectiveData;
}
