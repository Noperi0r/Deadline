// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "WeaponThrowBase.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEPROJECT_API AWeaponThrowBase : public AWeapon
{
	GENERATED_BODY()

private:
	AWeaponThrowBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowProtectedAccess = "true"))
	EThrowingTag ThrowingTag; // Needs to be set in BP

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowProtectedAccess = "true"))
	//class UStaticMeshComponent* ThrowingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowProtectedAccess = true))
	class UGeometryCollectionComponent* ThrowingMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation", meta = (AllowProtectedAccess = "true"))
	class UAnimMontage* Montage_Trigger;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowProtectedAccess = "true"))
	float ThrowPower;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowProtectedAccess = "true"))
	float ThrowAngle; // float or Fvector?

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowProtectedAccess = "true"))
	float ThrowLoudness;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowProtectedAccess = "true"))
	float ThrowLoudnessRange;
	
	UPROPERTY()
	bool bReadyForThrow;

	UPROPERTY()
	bool bHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise", meta = (AllowProtectedAccess = true))
	TSubclassOf<AActor> NoiseDisplayRef;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Noise", meta = (AllowProtectedAccess = true))
	AActor* NoiseDisplayer;

	UPROPERTY()
	class UMaterialInstanceDynamic* NoiseMatInst;

	FTimerHandle NoiseDisplayTimer;
	float NoiseColAlpha;
	float OriginalNoiseColAlpha;

public:
	virtual EThrowingTag GetThrowingTag();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void Attack() override;

	virtual bool CanAttack() override;

	virtual void PerformTrace() override;

	virtual void SubAction() override;

	virtual void ConvertPlayerController() override;

	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	virtual void MakeWeaponNoise(const FVector& Location) override;

private:
	UFUNCTION()
	void Launch();

	UFUNCTION()
	void UpdatePlayer();

	UFUNCTION()
	void UpdateSpline(const struct FPredictProjectilePathResult& PathResult);

	// Deprecated
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ChaosBreakHandler(const FChaosBreakEvent& BreakEvent);

	UFUNCTION()
	virtual void OnTriggerMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void SpawnNoiseSphere();

	UFUNCTION()
	void UpdateNoiseSphereAlpha(AActor* DisplayObj);
};
