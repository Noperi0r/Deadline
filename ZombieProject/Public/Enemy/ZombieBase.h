// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieData.h"
#include "ZombieBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZombieDeath, FZombieData, DeadZombieData);

UCLASS()
class ZOMBIEPROJECT_API AZombieBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 uuid;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnZombieDeath OnDead;
	
	UFUNCTION(BlueprintCallable,Category = "Events")
	void TriggerOnDeath();

	UFUNCTION()
	void DebugDele(FZombieData deadZombieData);

	UFUNCTION(BlueprintCallable,Category = "Events")
	void TargetToPlayer();

	UFUNCTION(BlueprintNativeEvent,Category = "Events")
	void Parried();
	void Parried_Implementation();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	float Threshold;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI")
	float TrunThreshold;

	UFUNCTION(BlueprintCallable)
	void Turn(FVector NoiseLoc);

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Setting")
	int32 QTECount;
};
