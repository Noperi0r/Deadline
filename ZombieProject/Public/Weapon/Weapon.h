// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

#define D(X,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT(X))); // For debugging, X muse be const FString
#define DI(I,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT("%d"),I));
#define DS(S,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT("%s"),S)); //For debugging with variable, X and Y muse be String

UENUM(BlueprintType)
enum class EWeaponTag : uint8
{
	NONE UMETA(DisplayName = "None"),
	MELEE UMETA(DisplayName = "MeleeWeapon"),
	RANGE UMETA(DisplayName = "RangeWeapon"),
	THROW UMETA(DisplayName = "ThrowWeapon")
};

UENUM(BlueprintType)
enum class EThrowingTag : uint8
{
	NONE UMETA(DisplayName = "None"),
	BRICK UMETA(DisplayName = "Brick"),
	GRENADE UMETA(DisplayName = "Grenade"),
	LAST UMETA(DisplayName = "Items enum last count")
};

UENUM(BlueprintType)
enum class EGunMode: uint8
{
	NONE UMETA(DisplayName = "None"),
	PISTOL UMETA(DisplayName = "Pistol"),
	RIFLE UMETA(DisplayName = "Rifle"),
};


UCLASS(Abstract)
class ZOMBIEPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowProtectedAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowProtectedAccess = "true"))
	class UAudioComponent* AttackSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowProtectedAccess = "true"))
	EWeaponTag ItemTag;

	UPROPERTY(BlueprintReadOnly, Category = "Noise", meta = (AllowProtectedAccess = true))
	class UBaseNoiseEmitterComponent* NoiseEmitter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise", meta = (AllowProtectedAccess = true))
	bool bDrawNoiseDebug;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Noise")
	float WeaponLoudness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Noise")
	float WeaponLoudnessRange; 

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class AZombiePlayerController* Controller;

public:
	//TSubclassOf<class AZombiePlayer> Player;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class AZombiePlayer* Player;

public:
	//UFUNCTION(BlueprintCallable)
	//virtual void Attack() PURE_VIRTUAL(AWeapon::Attack, ;);

	//UFUNCTION(BlueprintCallable)
	//virtual void OnEquip();

	//UFUNCTION(BlueprintCallable)
	//virtual void DisarmWeapon();

	UFUNCTION()
	virtual EWeaponTag GetItemTag();
	virtual int32 GetItemCode();
	virtual void SetItemTag(EWeaponTag Tag);
	virtual void SetItemCode(const int32 Code);
	virtual void SetPlayer(AZombiePlayer* ZombiePlayer);


	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void Attack() PURE_VIRTUAL(AWeapon::Attack, return;);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void SubAction() PURE_VIRTUAL(AWeapon::SubAction, return;);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual bool CanAttack() PURE_VIRTUAL(AWeapon::CanAttack, return true;);
	
	UFUNCTION(BlueprintCallable)
	virtual AWeapon* LoadWeaponData(AWeapon* Weapon) PURE_VIRTUAL(AWeapon::LoadWeaponData, return this;);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void PerformTrace() PURE_VIRTUAL(AWeapon::PerformTrace, return;);

	UFUNCTION()
	virtual void ConvertPlayerController() PURE_VIRTUAL(AWeapon::ConvertPlayerController, return;);

	UFUNCTION()
	virtual void MakeWeaponNoise(const FVector& Location) PURE_VIRTUAL(AWeapon::MakeWeaponNoise, return;);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_BringWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_Attack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* Montage_PutBackWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform PlacementTransform;

protected:
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) PURE_VIRTUAL(AWeapon::OnAttackMontageEnded, return;);



};
