// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestBase.h"
#include "GameFramework/Actor.h"
#include "QuestManager.generated.h"



UCLASS(Blueprintable)
class ZOMBIEPROJECT_API AQuestManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	TArray<TSoftObjectPtr<UQuestBase>> ArrayOfQuestObjects;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadQuestList();

	void SaveQuestList();

	UFUNCTION(BlueprintCallable)
	UQuestBase* GetQuestByName(FName QuestTypeName);

	UFUNCTION(BlueprintCallable)
	UQuestBase* GetQuestById(FGuid QuestId);

	UFUNCTION(BlueprintCallable)
	bool CanAcceptQuestByName(FName QuestTypeName);

	UFUNCTION(BlueprintCallable)
	bool CanAcceptQuestById(FGuid QuestId);


};
