// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/EInteractionType.h"
#include "UObject/Interface.h"
#include "InteractionUI.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractionUI : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIEPROJECT_API IInteractionUI
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void UpdateUI(EInteractionType InteractionType, bool bIsInteractable);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
	void ClearUI();
};
