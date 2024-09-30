// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera\CameraComponent.h"
#include "Components\CapsuleComponent.h"
//#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SpotLightComponent.h"
#include "Noise/BaseNoiseEmitterComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "ZombiePlayerInputConfigData.h"
#include "Interaction/InteractableInterface.h"
#include "Weapon\Weapon.h"
#include "Weapon\WeaponGunBase.h"
//#include "InventoryComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ZombiePlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);

UENUM(BlueprintType)
enum class EPlayerMode : uint8 {
	None		UMETA(DisplayName = "None"),
	MeleeMode	UMETA(DisplayName = "Melee"),
	RangeMode	UMETA(DisplayName = "Range"),
	ThrowMode	UMETA(DisplayName = "Throw"),
};

UENUM(BlueprintType)
enum class EPlayerCamShake : uint8
{
	Idle UMETA(DisplayName = "CamShake Idle"),
	Walk UMETA(DisplayName = "CamShake Walk"),
	Run UMETA(DisplayName = "CamShake Run"),
	BatAttack UMETA(DisplayName = "CamShake Bat Attack"),
	RifleAttack UMETA(DisplayName = "CamShake Rifle Attack"),
	Execute UMETA(DisplayName = "CamShake Execute"),
	Kick UMETA(DisplayName = "CamShake Kick"),
	Hit UMETA(DisplayName = "CamShake Hit")
};

UCLASS()
class ZOMBIEPROJECT_API AZombiePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	int32 Hp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	int32 MaxHp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float Stamina;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float MaxStamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bCanRecoverStamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float IntervalToRecoverStamina;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float StaminaRecoveryAmount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float SprintStaminaAmount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bIsSprinting;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bIsMovingBackward;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CrouchHalfHeight;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CapsuleHalfHeight;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bOnCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	bool bCanQTE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int32 QTECount;

	/*
	 * Player Speed
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CrouchSpeed;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float BackCrouchSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float WalkSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float BackWalkSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float SprintSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float BackSprintSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FVector2D MovementInput;

	/*
	 * Player Loudness
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CrouchLoudness;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float WalkLoudness;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float SprintLoudness;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float ParkourLoudness;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CurrentLoudness;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float LoudnessRange;

	/*
	 *
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float StiffnessTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bOnHit;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	bool bOnDead;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float StandFov;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CrouchFov;
	
	FTimerHandle HitTimer;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	EPlayerMode CurrentPlayerMode;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float CameraSensitivity;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
	float MaxCameraLookSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float FlashlightIntensity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interact")
	float TraceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	AActor* TracedActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	UPrimitiveComponent* TracedComp;

	/*
	 * Player Parkour Property
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawParkourDebug;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	bool bOnParkour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float ParkourDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	TArray<float> MaxParkourHeights;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	float MaxParkourWidth;
	FVector ParkourStartLocation;
	FVector ParkourMiddleLocation;
	FVector ParkourEndLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
	UMotionWarpingComponent* MotionWarping;

	/*
	 * Player Components
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ThrowLocation;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpotLightComponent* Flashlight;
	float FlashlightInnerConeAngle;
	float FlashlightOutConeAngle;*/

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USplineComponent* ThrowPathSpline;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ThrowEndSphere;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Shake")
	TArray<TSubclassOf<UCameraShakeBase>> CamShakes;


public:

	/*
	*	PLAYER WEAPON INTERACTION INCLUDING INVENTORY
	*/

	// IMPORTANT : SET IN BP
	// 0: Brick, 1: Grenade 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<AWeapon>> Throwings; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsAiming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bDoingAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawExecuteDebug;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bCanExecute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bOnExecute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bCanThrow;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bCannotAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName ExecutableBone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ExecutableBoneDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ExecutableDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ExecutableDegree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	int32 KickDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float ThrowingForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TArray<USplineMeshComponent*> ThrowSplineMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float IntervalToAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* CurrentWeapon;

	class AWeapon* WeaponToSwap;

	UPROPERTY(BlueprintReadWrite, Category = "Information")
	class UInventoryComponent* PlayerInventory;

	UPROPERTY(BlueprintReadWrite, Category = "Information")
	
	int32 RecentMeleeWeaponIndex;
	int32 Num_MaxMeleeWeapon;

	int32 RecentRangeWeaponIndex; // 0, 1 Only
	int32 Num_MaxRangeWeapon; // Used for computing remainder, meaning it should be set to 2 if 2 range weapons are available(to get remainder 0,1)

	UPROPERTY(BlueprintReadWrite, Category = "Information")
	int32 RecentThrowingWeaponIndex; // 0, 1, 2 Only
	int32 Num_MaxThrowingTypes;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	int32 SwingCount;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle FollowRotationTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawNoiseDebug;
	UPROPERTY(BlueprintReadOnly)
	class UBaseNoiseEmitterComponent* NoiseEmitter;

public:
	// Sets default values for this character's properties
	AZombiePlayer();

	~AZombiePlayer();

private:
	// Recover Player stamina
	UFUNCTION(BlueprintCallable)
	void RecoverStamina();

	// Check actor with the line trace
	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool LineTrace(const FVector Start, const FVector End, FHitResult& HitResult, const TArray<int>& ObjectTypes, const TArray<AActor* >&IgnoreActors, const bool bDebug=false);
	UFUNCTION()
	void LineTraceFromCamera();
	UFUNCTION()
	void LineTraceFromGround();
	
	// Set the traced actor outlined
	UFUNCTION()
	void SetActorHighlight(const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool Hit(AActor* DamageCauser, int32 Damage);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool Death();

public:
	UFUNCTION(BlueprintCallable)
	void RunCamShake(EPlayerCamShake Mode);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Check if Player can do actions
	UFUNCTION()
	bool CanAct();

	// Check if Player can do kick
	UFUNCTION()
	bool CanKick();

	// Check if Player can sprint
	UFUNCTION(BlueprintCallable)
	bool CanSprint();

	UFUNCTION()
	void SetPlayerThrowFlag(bool bFlag);

	UFUNCTION()
	bool CanPlayerThrow();

	// Wait a few seconds to recover stamina
	UFUNCTION(BlueprintCallable)
	void WaitToRecoverStamina();

	// Check whether the traced actor is interactable or not
	UFUNCTION()
	bool IsInteractable();

	UFUNCTION(BlueprintNativeEvent)
	void EscapeQTE();
	void EscapeQTE_Implementation();
	



	/*
	* Player Attack
	*/

	void WeaponChange(const int32 Type, const int32 WeaponIndex);
	void EquipWeapon();
	UFUNCTION()
	void DisarmWeapon(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void SetNextWeaponIndex(const int32 Type);

	UFUNCTION(BlueprintCallable)
	void LootWeapon(AWeapon* Weapon);

	void DropWeapon(const int32 WeaponIndex, const int32 WeaponSlot);

	bool CanAttack();

	void AssignWeaponToPlayer(AWeapon* Weapon); // Assign weapon to the player => Set CurrentWeapon

	void SetActiveWeapon(AWeapon* Weapon, bool bIsSet); // Lets the weapon invisible/visible on the world 
	
	void Attack();

	bool IsExecutable();

	void ResetPlayerVariables();

public:
	/*
	*	ZombiePlayer Animation Montages
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	class UZombiePlayerAnimInstance* PlayerAnimInstance;

	// Parkour Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> ParkourMontages;

	// Kick Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* KickMontage;

	// Execution Montages
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> ExecuteMontages;

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponHit OnWeaponHit;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGunFire OnGunFire;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponChanged OnWeaponChanged;


private:
	void SetMontageDelegate(UAnimMontage* Montage, FOnMontageEnded InDelegate, FName FunctionName);

	UFUNCTION()
	void OnSwapProcessEnded(UAnimMontage* Montage, bool bInterrupted);

};