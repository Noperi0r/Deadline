// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponMeleeBase.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API AWeaponMeleeBase : public AWeapon
{
	GENERATED_BODY()
public:
	AWeaponMeleeBase();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowProtectedAccess = "true"))
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttackProperty")
	int32 MaxAttackNumber;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ExecuteMontageIdx;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	virtual void Attack() override;
	virtual void SubAction() override;
	UFUNCTION(BlueprintNativeEvent)
	void SubAction_Internal();
	virtual void SubAction_Internal_Implementation();
	virtual bool CanAttack() override;
	virtual void PerformTrace() override;
	virtual void ConvertPlayerController() override;


public:
	bool bCollisionOn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_Attack2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_Attack3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_Attack4;

protected:
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
};
