#include "Quest/QuestManager.h"

// Sets default values
AQuestManager::AQuestManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();

	LoadQuestList();
}

void SaveQuestList()
{
	
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestManager::LoadQuestList()
{
	// // Get Current Level Name
	// FString CurrentLevelName = GetWorld()->GetMapName(); 
	// // Load the Data Table of QuestList by Level Name (Unimplemented)
	// UDataTable* QuestDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Quests/QuestDataTable.QuestDataTable"));
	//
	// // Check if the QuestDataTable is loaded
	// ensureMsgf(QuestDataTable != nullptr, TEXT("QuestListDataTable not found"));
	// check(QuestDataTable != nullptr)
}

void AQuestManager::SaveQuestList()

{
}

UQuestBase* AQuestManager::GetQuestByName(FName QuestTypeName)
{
	// Find the quest by name
	for (TSoftObjectPtr<UQuestBase> Quest : ArrayOfQuestObjects)
	{
		if (Quest->GetQuestName() == QuestTypeName)
		{
			return Quest.Get();
		}
	}
	return nullptr;
}

UQuestBase* AQuestManager::GetQuestById(FGuid QuestId)
{
	// Find the quest by id
	for (TSoftObjectPtr<UQuestBase> Quest  : ArrayOfQuestObjects)
	{
		if (Quest->GetQuestId() == QuestId)
		{
			return Quest.Get();
		}
	}
	return nullptr;
}

bool AQuestManager::CanAcceptQuestByName(FName QuestTypeName)
{
	// Check if the quest can be accepted
	auto quest = GetQuestByName(QuestTypeName);
	return quest->CanAcceptQuest();
}

bool AQuestManager::CanAcceptQuestById(FGuid QuestId)
{
	// Check if the quest can be accepted
	auto quest = GetQuestById(QuestId);
	return quest->CanComplete();
}
