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
#include "Interaction/InteractableInterface.h"
#include "Weapon\Weapon.h"
#include "Weapon\WeaponGunBase.h"
//#include "InventoryComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ZombiePlayerInputConfigData.h"

#include "ZombiePlayer.generated.h"

/** Delegate triggered when weapon hits something */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponHit);
/** Delegate triggered when gun is fired */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunFire);
/** Delegate triggered when weapon is changed */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponChanged);
/** Delegate triggered when player dies */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDead);

/**
 * Enum defining different combat modes for the player
 * Controls what actions are available and how input is interpreted
 */
UENUM(BlueprintType)
enum class EPlayerMode : uint8 {
    None        UMETA(DisplayName = "None"),     // Default state, no special combat actions
    MeleeMode   UMETA(DisplayName = "Melee"),    // Close combat with melee weapons
    RangeMode   UMETA(DisplayName = "Range"),    // Ranged combat with firearms
    ThrowMode   UMETA(DisplayName = "Throw"),    // Throwing items or weapons
};

/**
 * Enum defining different types of camera shake effects
 * Used to provide visual feedback for various player actions
 */
UENUM(BlueprintType)
enum class EPlayerCamShake : uint8
{
    Idle UMETA(DisplayName = "CamShake Idle"),           // Subtle idle movement
    Walk UMETA(DisplayName = "CamShake Walk"),           // Walking movement shake
    Run UMETA(DisplayName = "CamShake Run"),             // More intense running shake
    BatAttack UMETA(DisplayName = "CamShake Bat Attack"), // Impact shake for bat attacks
    RifleAttack UMETA(DisplayName = "CamShake Rifle Attack"), // Recoil shake for rifle
    Execute UMETA(DisplayName = "CamShake Execute"),     // Execution move camera effect
    Kick UMETA(DisplayName = "CamShake Kick"),          // Kick impact shake
    Hit UMETA(DisplayName = "CamShake Hit")             // When player takes damage
};

/**
 * Enum defining different types of parkour movements
 * Controls animation and movement behavior during parkour
 */
UENUM(BlueprintType)
enum class EParkourType : uint8
{
    Vaulting UMETA(DisplayName = "Parkour Vault"),      // Quick vault over low obstacles
    Climbing UMETA(DisplayName = "Parkour Climbing"),    // Climbing up higher obstacles
    None UMETA(DisplayName = "Parkour None"),           // No parkour action
};

/**
 * Main player character class for the zombie game
 * Handles all player functionality including movement, combat, parkour, and interaction systems
 */
UCLASS()
class ZOMBIEPROJECT_API AZombiePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	/** 
	 * Base constructor
	 * Initializes default values for player character
	 */
	AZombiePlayer();

	/** 
	 * Destructor
	 * Cleans up any allocated resources
	 */
	virtual ~AZombiePlayer() override;

    // CORE STATS
    //===========

    /** Current health points of the player */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    int32 Hp;

    /** Maximum health points the player can have */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    int32 MaxHp;

    /** Current stamina points used for sprinting and special actions */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float Stamina;
    
    /** Maximum stamina points the player can have */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float MaxStamina;

    // STAMINA SYSTEM
    //==============

    /** Flag indicating if stamina can be recovered */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bCanRecoverStamina;

    /** Time in seconds between stamina recovery ticks */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float IntervalToRecoverStamina;

    /** Amount of stamina recovered per tick */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float StaminaRecoveryAmount;

    /** Amount of stamina consumed per second while sprinting */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float SprintStaminaAmount;

    // MOVEMENT STATE FLAGS
    //===================

    /** Flag indicating if the player is currently sprinting */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bIsSprinting;

    /** Flag indicating if the player is moving backward */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bIsMovingBackward;

    /** Capsule collision half-height when crouching */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CrouchHalfHeight;

    /** Default capsule collision half-height when standing */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CapsuleHalfHeight;

    /** Flag indicating if player is currently crouched */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bOnCrouched;

	 /** Flag indicating if player can perform QTE actions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
    bool bCanQTE;

    /** Counter for successful QTE inputs */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
    int32 QTECount;

    // MOVEMENT SPEED SETTINGS
    //======================

    /** Movement speed while in crouch state */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CrouchSpeed;
    
    /** Movement speed while crouching and moving backward */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float BackCrouchSpeed;

    /** Base walking speed */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float WalkSpeed;

    /** Walking speed while moving backward */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float BackWalkSpeed;

    /** Running speed while sprinting */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float SprintSpeed;

    /** Sprint speed while moving backward */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float BackSprintSpeed;

    /** Current 2D movement input vector */
    UPROPERTY(BlueprintReadWrite, Category = "Input")
    FVector2D MovementInput;

    // SOUND/NOISE SYSTEM
    //==================

    /** Noise level generated while crouch-walking */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CrouchLoudness;

    /** Noise level generated while walking */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float WalkLoudness;

    /** Noise level generated while sprinting */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float SprintLoudness;

    /** Noise level generated during parkour actions */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float ParkourLoudness;

    /** Current noise level being emitted by the player */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CurrentLoudness;

    /** Maximum distance at which player noise can be heard */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float LoudnessRange;

    // COMBAT SYSTEM
    //=============

    /** Duration player remains stiffened after being hit */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float StiffnessTime;

    /** Flag indicating player is currently in hit reaction */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bOnHit;

    /** Flag indicating player has died */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    bool bOnDead;

    /** Timer handle for managing hit reaction duration */
    FTimerHandle HitTimer;

    /** Current active combat mode (None/Melee/Range/Throw) */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    EPlayerMode CurrentPlayerMode;

    // CAMERA SYSTEM
    //=============

    /** Camera sensitivity for look/aim control */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CameraSensitivity;

    /** Maximum allowed camera look sensitivity */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float MaxCameraLookSensitivity;

    /** Field of view when standing */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float StandFov;

    /** Field of view when crouching */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Information")
    float CrouchFov;

    /** Intensity of player's flashlight */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
    float FlashlightIntensity;

    // INTERACTION SYSTEM
    //==================

    /** Maximum distance for interaction ray trace */
    UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Interact")
    float TraceDistance;

    /** Currently detected interactive actor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
    AActor* TracedActor;

    /** Currently detected interactive component */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
    UPrimitiveComponent* TracedComp;

    // PARKOUR SYSTEM PROPERTIES
    //========================

    /** Enable/disable parkour debug visualization */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDrawParkourDebug;

    /** Flag indicating player is performing parkour */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    bool bOnParkour;

    /** Maximum distance for parkour movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    float ParkourDistance;

    /** Array of maximum heights for different parkour movements */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    TArray<float> MaxParkourHeights;

    /** Maximum width for parkour movements */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    float MaxParkourWidth;
	/** Starting position for parkour movement */
    FVector ParkourStartLocation;

    /** Midpoint position for parkour movement trajectory */
    FVector ParkourMiddleLocation;

    /** End position for parkour movement */
    FVector ParkourEndLocation;

    /** Component handling motion warping for smooth parkour transitions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    UMotionWarpingComponent* MotionWarping;

    /** Current type of parkour movement being performed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parkour")
    EParkourType ParkourType;

    // CORE COMPONENTS
    //===============

    /** First person camera component */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UCameraComponent* Camera;

    /** Scene component defining throw origin point */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* ThrowLocation;

    /** Spline component for visualizing throw trajectory */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Throwing")
    class USplineComponent* ThrowPathSpline;
    
    /** Mesh component showing throw endpoint */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Throwing")
    class UStaticMeshComponent* ThrowEndSphere;

    /** Flag indicating player is ready to throw */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Throwing")
    bool bReadyForThrow;

    /** Flag indicating throw has been initiated */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Throwing")
    bool bThrowingLaunched;

    /** Knife mesh component */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Knife")
    class UStaticMeshComponent* KnifeMesh;

    // CAMERA SHAKE SYSTEM
    //===================

    /** Array of camera shake classes for different player actions */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera Shake")
    TArray<TSubclassOf<UCameraShakeBase>> CamShakes;

    // WEAPON SYSTEM
    //=============

    /** Array of throwable weapon classes (0: Brick, 1: Grenade) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<AWeapon>> Throwings;

    /** Flag indicating player is in aim mode */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    bool bIsAiming;

    /** Flag indicating player is performing attack action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    bool bDoingAttack;

    /** Enable/disable execution debug visualization */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDrawExecuteDebug;

    /** Flag indicating execution move is available */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bCanExecute;

    /** Flag indicating execution move is in progress */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bOnExecute;

    /** Flag indicating throw action is available */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    bool bCanThrow;

    /** Flag preventing player actions during certain states */
    UPROPERTY(BlueprintReadWrite, Category = "Animation")
    bool bCannotAction;

    /** Bone name used for execution move detection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName ExecutableBone;

    /** Distance threshold for execution bone detection */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ExecutableBoneDistance;

    /** Maximum distance for execution move */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ExecutableDistance;

    /** Maximum angle for execution move */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ExecutableDegree;

    /** Base damage for kick attack */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
    int32 KickDamage;

    /** Force applied to thrown weapons */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    float ThrowingForce;

    /** Array of spline meshes for throw trajectory visualization */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    TArray<USplineMeshComponent*> ThrowSplineMeshes;

    /** Cooldown time between attacks */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    float IntervalToAttack;

    /** Currently equipped weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    class AWeapon* CurrentWeapon;

    /** Weapon to be equipped next during weapon swap */
    class AWeapon* WeaponToSwap;

    /** Player's inventory component */
    UPROPERTY(BlueprintReadWrite, Category = "Information")
    class UInventoryComponent* PlayerInventory;

	/** Index of last used melee weapon */
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	int32 RecentMeleeWeaponIndex;

	/** Maximum number of melee weapons that can be carried */
	int32 Num_MaxMeleeWeapon;

	/** Index of last used ranged weapon (0 or 1) */
	int32 RecentRangeWeaponIndex;

	/** Maximum number of ranged weapons that can be carried (set to 2) */
	int32 Num_MaxRangeWeapon;

	/** Index of last used throwing weapon (0-2) */
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	int32 RecentThrowingWeaponIndex;

	/** Maximum number of throwing weapon types */
	int32 Num_MaxThrowingTypes;

	/** Counter for consecutive swing attacks */
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	int32 SwingCount;

	/** Timer for handling rotation following */
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle FollowRotationTimer;

	/** Enable/disable noise debug visualization */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDrawNoiseDebug;

	/** Component for handling noise emission */
	UPROPERTY(BlueprintReadOnly)
	class UBaseNoiseEmitterComponent* NoiseEmitter;

private:
	/** 
	 * Recovers player stamina over time
	 * Called periodically when stamina recovery is enabled
	 */
	UFUNCTION(BlueprintCallable)
	void RecoverStamina();

	/** 
	 * Performs a line trace for interaction detection
	 * @param Start - Starting point of trace
	 * @param End - End point of trace
	 * @param HitResult - Structure containing trace result
	 * @param ObjectTypes - Array of object types to trace against
	 * @param IgnoreActors - Array of actors to ignore in trace
	 * @param bDebug - Whether to draw debug lines
	 * @return True if trace hit something, false otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool LineTrace(const FVector Start, const FVector End, FHitResult& HitResult, 
				  const TArray<int>& ObjectTypes, const TArray<AActor*>&IgnoreActors, 
				  const bool bDebug=false);

	/** 
	 * Handles visual highlight of interactable objects
	 * @param Hit - Hit result containing traced object information
	 */
	UFUNCTION()
	void SetActorHighlight(const FHitResult& Hit);

	/** 
	 * Handles player taking damage
	 * @param DamageCauser - Actor causing the damage
	 * @param Damage - Amount of damage to apply
	 * @return True if damage was successfully applied
	 */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool Hit(AActor* DamageCauser, int32 Damage);

	/** 
	 * Handles player death state
	 * @return True if death handling was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool Death();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 
	 * Triggers appropriate camera shake effect
	 * @param Mode - Type of camera shake to play
	 */
	UFUNCTION(BlueprintCallable)
	void RunCamShake(EPlayerCamShake Mode);

	/** 
	 * Checks if player can perform actions
	 * @return True if player can perform actions
	 */
	UFUNCTION()
	bool CanAct();

	/** 
	 * Checks if player can perform kick attack
	 * @return True if kick attack is available
	 */
	UFUNCTION()
	bool CanKick();

	/** 
	 * Checks if player can sprint
	 * @return True if sprinting is available
	 */
	UFUNCTION(BlueprintCallable)
	bool CanSprint();

	/** 
	 * Sets player's throw state
	 * @param bFlag - New throw state
	 */
	UFUNCTION()
	void SetPlayerThrowFlag(bool bFlag);

	/** 
	 * Checks if player can perform throw action
	 * @return True if throw action is available
	 */
	UFUNCTION()
	bool CanPlayerThrow();

	// Wait a few seconds to recover stamina
	UFUNCTION(BlueprintCallable)
	void WaitToRecoverStamina();

	/** 
	* Checks if traced actor is interactable
	* @return True if actor implements interactable interface
	*/
	UFUNCTION()
	bool IsInteractable();

	/** 
	 * Handles QTE escape action
	 * Blueprint implementable event for QTE escape mechanics
	 */
	UFUNCTION(BlueprintNativeEvent)
	void EscapeQTE();
    
	/** 
	 * Native implementation of QTE escape
	 */
	void EscapeQTE_Implementation();
	
	/** 
	* Performs line trace from player camera for interaction
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LineTraceFromCamera();
	virtual void LineTraceFromCamera_Implementation();


	/** 
	 * Handles weapon switching
	 * @param Type - Type of weapon to switch to
	 * @param WeaponIndex - Index of weapon within type
	 */
	UFUNCTION(BlueprintCallable)
	void WeaponChange(const int32 Type, const int32 WeaponIndex);

	/** 
	 * Equips specified weapon
	 */
	UFUNCTION()
	void EquipWeapon();

	/** 
	 * Activates knife mesh
	 */
	UFUNCTION()
	void PutKnifeOn();

	/** 
	 * Deactivates knife mesh
	 */
	UFUNCTION()
	void PutKnifeOff();

	/** 
	 * Handles weapon disarming animation end
	 * @param Montage - Animation montage that finished
	 * @param bInterrupted - Whether animation was interrupted
	 */
	UFUNCTION()
	void DisarmWeapon(UAnimMontage* Montage, bool bInterrupted);

	/** 
	 * Updates weapon index for next weapon switch
	 * @param Type - Type of weapon to update index for
	 */
	UFUNCTION(BlueprintCallable)
	void SetNextWeaponIndex(const int32 Type);

	/** 
	 * Handles picking up new weapon
	 * @param Weapon - Weapon to be looted
	 */
	UFUNCTION(BlueprintCallable)
	void LootWeapon(AWeapon* Weapon);

	/** 
	 * Handles dropping current weapon
	 * @param WeaponIndex - Index of weapon to drop
	 * @param WeaponSlot - Slot containing weapon to drop
	 */
	void DropWeapon(const int32 WeaponIndex, const int32 WeaponSlot);

	/** 
	 * Checks if player can perform attack
	 * @return True if attack is available
	 */
	bool CanAttack();

	/** 
	 * Sets weapon as player's current weapon
	 * @param Weapon - Weapon to assign
	 */
	void AssignWeaponToPlayer(AWeapon* Weapon);

	/** 
	 * Toggles weapon visibility in world
	 * @param Weapon - Weapon to toggle
	 * @param bIsSet - Whether to show or hide weapon
	 */
	void SetActiveWeapon(AWeapon* Weapon, bool bIsSet);
	
	/** 
	 * Performs basic attack with current weapon
	 */
	void Attack();

	/** 
	 * Checks if execution move is available on target
	 * @return True if execution is possible
	 */
	bool IsExecutable();

	/** 
	 * Resets player state variables to default values
	 */
	void ResetPlayerVariables();

public:

	// ANIMATION SYSTEM
	//================

	/** Reference to player's animation instance */
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	class UZombiePlayerAnimInstance* PlayerAnimInstance;

	/** Array of parkour animation montages */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> ParkourMontages;

	/** Animation montage for kick attack */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* KickMontage;

	/** Array of execution animation montages */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> ExecuteMontages;

public:
	// DELEGATES
	//=========

	/** Delegate broadcast when weapon hits */
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponHit OnWeaponHit;

	/** Delegate broadcast when gun is fired */
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnGunFire OnGunFire;

	/** Delegate broadcast when weapon is changed */
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnWeaponChanged OnWeaponChanged;

	/** Delegate broadcast when player dies */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event")
	FOnPlayerDead OnPlayerDead;

private:
	/** 
	 * Sets up montage delegates
	 * @param Montage - Animation montage to set delegate for
	 * @param InDelegate - Delegate to bind
	 * @param FunctionName - Name of function to bind to
	 */
	void SetMontageDelegate(UAnimMontage* Montage, FOnMontageEnded InDelegate, FName FunctionName);

	/** 
	 * Handles end of weapon swap animation
	 * @param Montage - Animation montage that finished
	 * @param bInterrupted - Whether animation was interrupted
	 */
	UFUNCTION()
	void OnSwapProcessEnded(UAnimMontage* Montage, bool bInterrupted);
};