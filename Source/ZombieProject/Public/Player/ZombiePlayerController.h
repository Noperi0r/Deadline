// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GUI/InteractionUI.h"
#include "ZombiePlayerController.generated.h"
/*
 * 
 */

// Delegates
DECLARE_MULTICAST_DELEGATE_ThreeParams(FNoise_Dele, float Loudness, const FVector& NoiseLocation, float Range);

UCLASS()
class ZOMBIEPROJECT_API AZombiePlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class AZombiePlayer* ZombiePlayer;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	class UZombiePlayerInputConfigData* InputActions;



public:
	//UPROPERTY(EditDefaultsOnly, Category = "Noise")
	FNoise_Dele OnNoiseDelegate;
	
	UPROPERTY(EditDefaultsOnly, Category = "GUI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "GUI")
	UUserWidget* HUDWidget;

	UPROPERTY(BlueprintReadWrite, Category = "GUI")
	TScriptInterface<IInteractionUI> InteractionUI;
	
private:

	UFUNCTION(BlueprintCallable)
	void Idle();

	/*
	*	PLAYER MOVE
	*/

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void EndSprint();

	UFUNCTION(BlueprintCallable)
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Kick();

	/*
	* PLAYER INTERACT
	*/

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact();


	//UFUNCTION(BlueprintCallable, Category = "Interact")
	//void QTE();
	/*
	* PLAYER PARKOUR
	*/

	UFUNCTION(BlueprintCallable)
	void Parkour();

	/*
	*	PLAYER WEAPON INTERACTION
	*/
	UFUNCTION(BlueprintCallable)
	void PutWeapon(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void LClick();

	UFUNCTION(BlueprintCallable)
	void RClick();

	UFUNCTION(BlueprintCallable)
	void ReleaseThrow();


	UFUNCTION(BlueprintCallable)
	void Reload();

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DoCrouch();
	virtual void DoCrouch_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnOffFlashlight();
	virtual void OnOffFlashlight_Implementation();
	
	/*
	 * Player Noise Event
	 */
	UFUNCTION(BlueprintCallable)
	void MakeEnvNoise(float Loudness, const FVector& NoiseLocation, float Range);

	UFUNCTION(BlueprintCallable)
	void MakeNoiseEffect(float Loudness, const FVector& NoiseLocation, float Range);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

};
