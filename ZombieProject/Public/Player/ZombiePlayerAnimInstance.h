// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombiePlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API UZombiePlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	class AZombiePlayer* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Properties")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Properties")
	bool bIsAir;

public:
	virtual void NativeBeginPlay() override;
	
	//UFUNCTION(BlueprintCallable, Category = "Update Properties")
	//void UpdateProperties();
private:
	UFUNCTION()
	void AssasinationEndHandler();

public:
	/** Weapon Interactions */
	FOnMontageEnded AttackEndDelegate;
	FOnMontageEnded DisarmEndDelegate;
	FOnMontageEnded WeaponActionEndDelegate;

	/** Assasination interaction */
	FOnMontageEnded OnAssinationEnded;
};
