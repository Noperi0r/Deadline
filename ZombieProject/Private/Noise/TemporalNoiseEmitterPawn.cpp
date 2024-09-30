// Fill out your copyright notice in the Description page of Project Settings.

#include "Noise/TemporalNoiseEmitterPawn.h"

// Sets default values
ATemporalNoiseEmitterPawn::ATemporalNoiseEmitterPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void ATemporalNoiseEmitterPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATemporalNoiseEmitterPawn::Tick(float DeltaTime)
{
}

// Called to bind functionality to input
void ATemporalNoiseEmitterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

