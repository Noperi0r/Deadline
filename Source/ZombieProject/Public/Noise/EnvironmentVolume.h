// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Noise/EEnvironmentType.h"
#include "Components/BoxComponent.h"
#include "EnvironmentVolume.generated.h"

UCLASS()
class ZOMBIEPROJECT_API AEnvironmentVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnvironmentVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnvironmentVolume")
	UBoxComponent* EnvironmentBoundaryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvironmentVolume")
	EEnvironmentType EnvironmentType;

public:
	bool CheckPositionInVolume(const FVector& Position) const;

	EEnvironmentType GetEnvironmentType() const { return EnvironmentType; }
};
