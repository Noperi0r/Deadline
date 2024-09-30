// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "ZombiePlayerInputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UZombiePlayerInputConfigData : public UDataAsset
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputMove;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSprint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputLook;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputCrouch;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputFlashlight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputInteract;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputAttack;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSubAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputReload;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSwap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputParkour;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputKick;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputAttackBtnUp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputQTE;
};
