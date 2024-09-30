// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInteractionType.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"

#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */

class ZOMBIEPROJECT_API IInteractableInterface
{
	GENERATED_BODY()

public:
	// Check if the object can be interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	bool CanInteract(AActor* Interactor) const;

	// Perform the interaction
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	void PerformInteract(AActor* Interactor);

	// Enable Outline
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	void EnableOutline();

	// Disable Outline
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	void DisableOutline();

	/**
	 * Cast를 효율젝으로 하기 위해 EInteractionType을 반환하는 함수
	 * @return EInteractionType
	 *
	 * @warn return type can be changed to return a Enum or a GameplayTag 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	EInteractionType GetInteractTag() const;
};
