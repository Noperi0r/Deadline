// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponGunBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ZOMBIEPROJECT_API AWeaponGunBase : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponGunBase();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowProtectedAccess = "true"))
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowProtectedAccess = "true"))
	float RecoilCoefficient;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta = (AllowProtectedAccess = "true"))
	class USceneComponent* MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowProtectedAccess = "true"))
	class UAudioComponent* ReloadSound;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int CurrentBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int MaxBullet;

	//UPROPERTY(EditDefaultsOnly, Category = "Effects", meta = (AllowProtectedAccess = "true"))
	//class UNiagaraComponent* FireEffect;

public:
	virtual void Attack() override;
	virtual void SubAction() override;
	virtual bool CanAttack() override;
	virtual void PerformTrace() override;
	virtual void ConvertPlayerController() override;
	virtual void MakeWeaponNoise(const FVector& Location) override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintCallable)
	void FailFire();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_Reload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_AimAttack;

protected:
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	
	UFUNCTION()
	virtual void OnReloadProcessEnd(UAnimMontage* Montage, bool bInterrupted);
};
