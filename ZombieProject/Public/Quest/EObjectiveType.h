// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
    Kill UMETA(DisplayName = "Kill"),
	Collect UMETA(DisplayName = "Collect"),
	Interact UMETA(DisplayName = "Interact"),
	Reach UMETA(DisplayName = "Reach"),
	Defend UMETA(DisplayName = "Defend"),
	Deliver UMETA(DisplayName = "Deliver"),
};
