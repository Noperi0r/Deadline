// Fill out your copyright notice in the Description page of Project Settings.

#include "Noise/EnvironmentVolume.h"

// Sets default values
AEnvironmentVolume::AEnvironmentVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnvironmentType = EEnvironmentType::TE_Plains;
	EnvironmentBoundaryComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = EnvironmentBoundaryComponent;

	// Disable Collision
	EnvironmentBoundaryComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnvironmentBoundaryComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	
}

// Called when the game starts or when spawned
void AEnvironmentVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnvironmentVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AEnvironmentVolume::CheckPositionInVolume(const FVector& Position) const
{
	auto BoundingBox = EnvironmentBoundaryComponent->Bounds.GetBox();
	return BoundingBox.IsInside(Position);
}

