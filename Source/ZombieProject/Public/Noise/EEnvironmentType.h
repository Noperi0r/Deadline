// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnvironmentType
{
    TE_Plains UMETA(DisplayName = "Plains"),
	TE_Room UMETA(DisplayName = "Room"),
	TE_Sewer UMETA(DisplayName = "Sewer"),
	TE_Plaza UMETA(DisplayName = "Plaza"),
};
    