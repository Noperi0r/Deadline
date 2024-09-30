// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "Item/PickableActor.h"

#include "MediKitActor.generated.h"

UCLASS()
class ZOMBIEPROJECT_API AMediKitActor : public APickableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMediKitActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	bool CanInteract_Implementation(AActor* Interactor) const override;

	virtual FPickableObjectData GetItemData_Implementation();

private:
	FName Name = FName(TEXT("MediKit"));

	int32 ItemId = 201;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxCollision;
};
