// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GUI/InteractionUI.h"
#include "ZombiePlayerController.generated.h"
/*
 * 
 */

/** Delegate for handling noise events with loudness, location, and range parameters */
DECLARE_MULTICAST_DELEGATE_ThreeParams(FNoise_Dele, float Loudness, const FVector& NoiseLocation, float Range);

/**
 * Player controller class handling input, UI, and noise systems for the zombie game
 * Manages player input mapping, HUD elements, and environmental noise propagation
 */
UCLASS()
class ZOMBIEPROJECT_API AZombiePlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	/** Reference to controlled zombie player character */
	UPROPERTY()
	class AZombiePlayer* ZombiePlayer;

	/** Input mapping context for enhanced input system */
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	/** Configuration data asset containing input actions */
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UZombiePlayerInputConfigData* InputActions;


public:
	/** Delegate for broadcasting noise events */
	FNoise_Dele OnNoiseDelegate;
    
	/** Class for main HUD widget */
	UPROPERTY(EditDefaultsOnly, Category = "GUI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	/** Instance of main HUD widget */
	UPROPERTY(BlueprintReadWrite, Category = "GUI")
	UUserWidget* HUDWidget;

	/** Interface for interaction UI elements */
	UPROPERTY(BlueprintReadWrite, Category = "GUI")
	TScriptInterface<IInteractionUI> InteractionUI;

	/** Class for death screen widget */
	UPROPERTY(EditDefaultsOnly, Category = "GUI")
	TSubclassOf<UUserWidget> DeadWidgetClass;

	/** Instance of death screen widget */
	UPROPERTY(BlueprintReadWrite, Category = "GUI")
	UUserWidget* DeadWidget;
	
private:
	/** Sets player to idle state */
	UFUNCTION(BlueprintCallable)
	void Idle();

	// Movement Functions
	/** 
	 * Handles player movement input
	 * @param Value - Input action value containing movement vector
	 */
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	/** Begins player sprint */
	UFUNCTION(BlueprintCallable)
	void Sprint();

	/** Ends player sprint */
	UFUNCTION(BlueprintCallable)
	void EndSprint();

	/** 
	 * Handles camera look input
	 * @param Value - Input action value containing look vector
	 */
	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	/** Performs kick attack */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Kick();

	/** Handles interaction with objects */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact();

	/** Initiates parkour movement */
	UFUNCTION(BlueprintCallable)
	void Parkour();

	// Weapon System Functions
	/** 
	 * Handles weapon equip/unequip
	 * @param Value - Input action value for weapon selection
	 */
	UFUNCTION(BlueprintCallable)
	void PutWeapon(const FInputActionValue& Value);

	/** Handles left mouse button input */
	UFUNCTION(BlueprintCallable)
	void LClick();

	/** Handles right mouse button input */
	UFUNCTION(BlueprintCallable)
	void RClick();

	/** Handles throw release action */
	UFUNCTION(BlueprintCallable)
	void ReleaseThrow();

	/** Initiates weapon reload */
	UFUNCTION(BlueprintCallable)
	void Reload();

public:
	/** 
	 * Handles crouch toggle action
	 * Blueprint implementable event for crouch mechanics
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DoCrouch();
	virtual void DoCrouch_Implementation();

	/** 
	 * Handles flashlight toggle
	 * Blueprint implementable event for flashlight control
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnOffFlashlight();
	virtual void OnOffFlashlight_Implementation();
    
	// Noise System Functions
	/** 
	 * Creates environmental noise event
	 * @param Loudness - Volume level of noise
	 * @param NoiseLocation - World location of noise source
	 * @param Range - Maximum distance noise can be heard
	 */
	UFUNCTION(BlueprintCallable)
	void MakeEnvNoise(float Loudness, const FVector& NoiseLocation, float Range);

	/** 
	 * Creates noise effect visualization
	 * @param Loudness - Volume level of noise
	 * @param NoiseLocation - World location of noise source
	 * @param Range - Maximum distance noise can be heard
	 */
	UFUNCTION(BlueprintCallable)
	void MakeNoiseEffect(float Loudness, const FVector& NoiseLocation, float Range);
	
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	/** Sets up player input bindings */
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* PawnToPossess) override;

};
