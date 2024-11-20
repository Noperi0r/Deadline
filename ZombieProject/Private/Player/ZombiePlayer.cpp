
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components\SkeletalMeshComponent.h"
//#include "Camera\CameraShakeBase.h"
#include "CameraShakeWalk.h"
#include "Components\SplineComponent.h"
#include "Enemy\ZombieBase.h"
#include "Weapon\WeaponThrowBase.h"
#include "InventoryComponent.h"
#include "Engine/DamageEvents.h"
#include "Player/ZombiePlayerController.h"


#define _MESH_ACTIVATE_

// Sets default values
AZombiePlayer::AZombiePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup stats
	Hp = 100;
	MaxHp = 100;

	Stamina = 100;
	MaxStamina = 100;
	bCanRecoverStamina = true;
	IntervalToRecoverStamina = 1.0f;
	StaminaRecoveryAmount = 1.0f;
	SprintStaminaAmount = 1.0f;
	bIsSprinting = false;

	bIsMovingBackward = false;

	// Setup height
	CrouchHalfHeight = 88.0f;
	CapsuleHalfHeight = 88.0f;

	bOnCrouched = false;

	// Setup Speed
	CrouchSpeed = 70.0f;
	BackCrouchSpeed = 50.0f;
	WalkSpeed = 150.0f;
	BackWalkSpeed = 100.0f;
	SprintSpeed = 600.0f;
	BackSprintSpeed = 300.0f;

	// Setup Loudness
	CrouchLoudness = 1.0f; // default 1.0f
	WalkLoudness = 1.0f; // default 3.0f
	SprintLoudness = 6.0f; // default 6.0f
	ParkourLoudness = 7.0f; // default 7.0f
	CurrentLoudness = 0.0f;
	LoudnessRange = 500.0f;

	CameraSensitivity = 0.25f;

	MaxCameraLookSensitivity = 5.5f;

	StiffnessTime = 0.5f;
	bOnHit = false;
	bOnDead = false;
	bCanQTE = true;

	StandFov = 90.0f;
	CrouchFov = 70.0f;

	// Setup Trace setting
	TraceDistance = 400.0f;

	// Setup Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	//Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(GetMesh());
	Camera->SetupAttachment(GetMesh(),"head");
	//Camera->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform,"head");
	//Camera->AddRelativeLocation(FVector(-39.6f, 1.75f, 64.0f));

	ThrowPathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Throw Path Spline"));
	ThrowEndSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Throw Path Mesh"));

	// Setup Knife 
	KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Knife Mesh"));
	KnifeMesh->SetupAttachment(GetMesh(), FName("weaponsocket_r"));
	KnifeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	KnifeMesh->bCastDynamicShadow = false;
	KnifeMesh->CastShadow = false;
	KnifeMesh->SetOnlyOwnerSee(true);
	
	// Setup Throw Location
	ThrowLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Throw Location"));
	ThrowLocation->SetupAttachment(Camera);
	
	// Setup Mesh
	GetMesh()->SetupAttachment(GetRootComponent());
    GetMesh()->bCastDynamicShadow = false;
    GetMesh()->CastShadow = false;
    //GetMesh()->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
    //GetMesh()->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
    GetMesh()->SetOnlyOwnerSee(true);
	 
	// Setup Parkour
	bDrawParkourDebug = false;
    bOnParkour = false;
    //bCanParkour = false;
    ParkourDistance = 100;
	//MaxParkourHeights;
	MaxParkourWidth = 200;
	//CameraHeightFromGround = 0;
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

	// Setup Flashlight
	/*Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(Camera);
	FlashlightInnerConeAngle = 0;
	FlashlightOutConeAngle = 20.0f;
	FlashlightIntensity = 50000.0f;
	Flashlight->SetInnerConeAngle(FlashlightInnerConeAngle);
	Flashlight->SetOuterConeAngle(FlashlightOutConeAngle);
	Flashlight->SetIntensity(FlashlightIntensity);*/
	FlashlightIntensity = 10.0; // 10.0lms

	// TODO: Setup ZombiePlayer CurrentWeapon (Default: Melee Bat)

	// Setup Inventory
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	
	RecentMeleeWeaponIndex = 0;
	RecentRangeWeaponIndex = 0;
	RecentThrowingWeaponIndex = 0;
	
	Num_MaxMeleeWeapon = PlayerInventory->CheckPrimaryWeaponNum(MELEEINDEX);
	Num_MaxRangeWeapon = PlayerInventory->CheckPrimaryWeaponNum(RANGEINDEX); 
	// TODO: Apply CheckPrimaryWeaponNum
	//Num_MaxThrowingTypes = (int32)EThrowingTag::LAST - 1;
	Num_MaxThrowingTypes = PlayerInventory->CheckThrowingTypesNum();

	// Setup Attack property
	bDoingAttack = false;
	bCanExecute = false;
	bOnExecute = false;
	bDrawExecuteDebug = false;

	// Execution property
	ExecutableBone = "spine_03";
	ExecutableBoneDistance = 65.0f;
	ExecutableDistance = 120.0f;
	ExecutableDegree = 0.99f;

	KickDamage = 5;

	ThrowingForce = 2500.0f;

	IntervalToAttack = 0.1f;

	SwingCount = 0;

	KickMontage = CreateDefaultSubobject<UAnimMontage>(TEXT("Kick Montage"));

	bDrawNoiseDebug = false;
	NoiseEmitter = CreateDefaultSubobject<UBaseNoiseEmitterComponent>(TEXT("Noise Emitter"));
}

AZombiePlayer::~AZombiePlayer()
{
	
}

// Called when the game starts or when spawned
void AZombiePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup parkour
	// CameraHeightFromGround = Camera->GetComponentLocation().Z - 20; //- FpsMesh->GetBoneLocation("pelvis",EBoneSpaces::WorldSpace).Z;

	// Setup Crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Setup Movement
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Setup Camera
	//Camera->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "CameraSocket");
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -60.0f;
	Camera->SetFieldOfView(StandFov);

	// Setup Animation
	PlayerAnimInstance = Cast<UZombiePlayerAnimInstance>(GetMesh()->GetAnimInstance());
	//PlayerAnimInstance = Cast<UZombiePlayerAnimInstance>(FpsMesh->GetAnimInstance());
	
	check(PlayerAnimInstance);

	// Default Player mode
	CurrentPlayerMode = EPlayerMode::None;
	//WeaponChange(THROWINDEX, RecentThrowingWeaponIndex); // Default when game starts

	UE_LOG(LogTemp, Warning, TEXT("Currrent Level Name: %s"), *GetWorld()->GetName());

	KnifeMesh->SetHiddenInGame(true);
}

// Called every frame
void AZombiePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanAct()) {
		// Line trace for general case
		LineTraceFromCamera();
	}
	RecoverStamina();

	//DrawDebugSphere(GetWorld(),ParkourEndLocation,20,12,FColor::Red, false, 10.0f);
}

// Called to bind functionality to input
void AZombiePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Recover player stamina
void AZombiePlayer::RecoverStamina() {
	if (Stamina >= MaxStamina) return;
	if (!bCanRecoverStamina) return;

	Stamina += StaminaRecoveryAmount;
	Stamina = Stamina > MaxStamina ? MaxStamina : Stamina;
}

void AZombiePlayer::WaitToRecoverStamina() {
	FTimerHandle TempTimer;

	// Check if the current world or player is disposed (restart or change level)
	if(this == nullptr || GetWorld() == nullptr)
		return;
	
	if (GetWorld()->GetTimerManager().GetTimerRemaining(TempTimer) <= 0) {
		if(this == nullptr || GetWorld() == nullptr)
			return;
		GetWorld()->GetTimerManager().SetTimer(TempTimer, [&]()
			{
				if(this == nullptr || GetWorld() == nullptr)
					return;
				bCanRecoverStamina = true;
				GetWorld()->GetTimerManager().ClearTimer(TempTimer);
			}, IntervalToRecoverStamina, false);
	}
}

bool AZombiePlayer::LineTrace(const FVector Start, const FVector End, FHitResult& HitResult, const TArray<int>& ObjectTypes, const TArray<AActor* >&IgnoreActors, const bool bDebug) {
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	for(int i=0; i<ObjectTypes.Num(); i++)
	{
		const ECollisionChannel CollisionChannel = static_cast<ECollisionChannel>(ObjectTypes[i]);
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(CollisionChannel));
	}
	
	EDrawDebugTrace::Type DebueTrace = EDrawDebugTrace::None;
	if(bDebug)
	{
		DebueTrace = EDrawDebugTrace::ForOneFrame;
	}
	
	return 	UKismetSystemLibrary::LineTraceSingleForObjects(_getUObject(), Start, End,
		ObjectTypesArray, false, IgnoreActors, DebueTrace, HitResult, true);
}

void AZombiePlayer::LineTraceFromCamera_Implementation()
{
	// Line trace from the camera
	FHitResult Hit;
	const FVector CamStart = Camera->GetComponentLocation();
	const FVector CamEnd = CamStart + Camera->GetForwardVector() * TraceDistance;
	if(CanAct())
	{
		if (LineTrace(CamStart, CamEnd, Hit, {2,3}, {}, false))
		{
			TracedActor = Hit.GetActor();
			TracedComp = Hit.GetComponent();
			if (IsInteractable()) { SetActorHighlight(Hit); }
			if (class AZombieBase* tracedZombie = Cast<AZombieBase>(TracedActor))
			{
				FVector ZombieLocation = tracedZombie->GetMesh()->GetBoneLocation(ExecutableBone,EBoneSpaces::WorldSpace);
				FVector PlayerLocation = GetActorLocation();
				float BoneDist = (Hit.Location-ZombieLocation).Length();
			
				FVector TempZombieLocation = ZombieLocation;
				TempZombieLocation.Z = 0;
				FVector TempPlayerLocation = PlayerLocation;
				TempPlayerLocation.Z = 0;
				float LocationDist = (TempPlayerLocation-TempZombieLocation).Length();
				
				FVector playerVector = GetActorForwardVector();
				FVector zombieVector = TracedActor->GetActorForwardVector();
				float degree = FMath::Acos(playerVector.Dot(zombieVector)/(playerVector.Length()*zombieVector.Length()));
				bCanExecute = (
					BoneDist <= ExecutableBoneDistance
					&& LocationDist <= ExecutableDistance
					&& degree <= ExecutableDegree);
			
				//UE_LOG(LogTemp, Warning, TEXT("Diff: %f, Dist: %f, Degree : %f"), BoneDist, LocationDist, degree);
				FColor ExecutionColor = FColor::Red;
				if(bCanExecute)
				{
					ExecutionColor = FColor::Green;
				}
				if(bDrawExecuteDebug)
				{
					DrawDebugSphere(GetWorld(),Hit.Location,2,12,FColor::Blue);
					DrawDebugSphere(GetWorld(),ZombieLocation,20, 12, ExecutionColor);
				}
			}
		}
		else {
			if (TracedComp != nullptr) {
				TracedComp->SetRenderCustomDepth(false);

				TracedComp = nullptr;
				TracedActor = nullptr;

				bCanExecute = false;
			}
		}
	}
}

void AZombiePlayer::SetActorHighlight(const FHitResult& Hit) {
	// Remove traced actor highlight previously
	if (TracedComp != nullptr && Hit.GetComponent() != TracedComp) {
		TracedComp->SetRenderCustomDepth(false);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Highlighted Actor: %s"), *TracedActor->GetName()));

	// when StaticMeshComponent not loaded directly 
	if (!Cast<UStaticMeshComponent>(TracedComp)) {
		UActorComponent* StaticMeshComp = TracedActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
		if (StaticMeshComp != nullptr) {
			TracedComp = Cast<UPrimitiveComponent>(StaticMeshComp);
		}
	}

	// when to get SkeletalMeshComponent
	//if (!Cast<USkeletalMeshComponent>(TracedComp)) {
	//	UActorComponent* SkeletalMeshComp = TracedActor->GetComponentByClass(USkeletalMeshComponent::StaticClass());
	//	if (SkeletalMeshComp != nullptr) {
	//		TracedComp = Cast<UPrimitiveComponent>(SkeletalMeshComp);
	//	}
	//}

	TracedComp->SetRenderCustomDepth(true);
}

bool AZombiePlayer::CanAct() {
	return Controller && !bOnParkour && !bOnExecute && !bOnHit && !bOnDead && bCanQTE;
}

bool AZombiePlayer::CanKick()
{
	return CanAct() && !bDoingAttack && !bCannotAction && !bOnCrouched;
}

bool AZombiePlayer::CanSprint() {
	return CanAct() && !bOnCrouched && Stamina >= SprintStaminaAmount;
}

void AZombiePlayer::SetPlayerThrowFlag(bool bFlag)
{
	bCanThrow = bFlag;
}

bool AZombiePlayer::CanPlayerThrow()
{
	return bCanThrow;
}

bool AZombiePlayer::IsInteractable() {
	if (TracedActor == nullptr) return false;
	return TracedActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass());
}

bool AZombiePlayer::Hit(AActor* DamageCauser, int32 Damage) {
	// Can't play Hit when the player is dying
	if (bOnDead) return false;
	
	// Apply damage
	Hp -= Damage;
	//bOnHit = true;

	// BP?�� TakeDamage ?��?���? ?��출하?�� UI�? ?��?��?��?�� ?��?��
	UGameplayStatics::ApplyDamage(this, Damage, DamageCauser->GetInstigatorController(), DamageCauser, nullptr);
	
	// Play Hit Anim Montage
	//GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);

	// Run Hit CamShake
	RunCamShake(EPlayerCamShake::Hit);
	
	UE_LOG(LogTemp, Warning, TEXT("Player Hit Invoked"));

	// During Hit Anim play, Cannot restore hp
	if (Hp <= 0) {
		Hp = 0;
		Death();
	}
	// Temporary Timer code. It will be replaced by AnimNotifySTate
	/*
	else {
		if (GetWorld()->GetTimerManager().GetTimerRemaining(HitTimer) <= 0) {
			GetWorld()->GetTimerManager().SetTimer(HitTimer, [&]()
			{
					bOnHit = false;
					GetWorld()->GetTimerManager().ClearTimer(HitTimer);
			}, StiffnessTime, false);
		}
	}
	*/
	return true;
}

bool AZombiePlayer::Death() {
	// Play Death anim
	UE_LOG(LogTemp, Warning, TEXT("Player Death Invoked"));
	// Check if the current world or player is disposed (restart or change level)
	if(this == nullptr || GetWorld() == nullptr)
		return false;

	bOnDead = true;
	FTimerHandle Timer;
	if (GetWorld()->GetTimerManager().GetTimerRemaining(Timer) <= 0) {
		if(this == nullptr || GetWorld() == nullptr)
			return false;
		GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
			{
			if(this == nullptr || GetWorld() == nullptr)
				return;
				UE_LOG(LogTemp, Warning, TEXT("Player Death Timer End Invoked"));
				OnPlayerDead.Broadcast();
				// UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
				GetWorld()->GetTimerManager().ClearTimer(Timer);
			}, 0.1f, false);
	}
	return true;
}

void AZombiePlayer::RunCamShake(EPlayerCamShake Mode)
{
	if ((int)Mode >= CamShakes.Num()) {
		UE_LOG(LogTemp, Error, TEXT("Out of CamShakes bound"));
		return;
	}
	if (!CamShakes[(int)Mode]) {
		UE_LOG(LogTemp, Error, TEXT("CamShake not found"));
		return;
	}
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CamShakes[(int)Mode], 1);
}

void AZombiePlayer::EscapeQTE_Implementation()
{
	//bp에서 구현
}

void AZombiePlayer::WeaponChange(const int32 Type, const int32 WeaponIndex)
{
	if (Type == THROWINDEX && PlayerInventory->bNoThrowings)
	{
		UE_LOG(LogTemp, Warning, TEXT("No throwings in inventory currently."));
		return;
	}

	WeaponToSwap = PlayerInventory->Inventory_GetWeapon(Type, WeaponIndex);

	if (!WeaponToSwap)
	{
		UE_LOG(LogTemp, Error, TEXT("No weapon in inventory. RETURNED"));

		if (CurrentPlayerMode == EPlayerMode::MeleeMode)
			return;
		else if (CurrentPlayerMode == EPlayerMode::ThrowMode)
		{
			CurrentPlayerMode = EPlayerMode::None;
			ResetPlayerVariables();
		}

		if(bThrowingLaunched)
			bThrowingLaunched = false;

		return;
	}

	switch (Type)
	{
		case MELEEINDEX:
			CurrentPlayerMode = EPlayerMode::MeleeMode;
			break;
		case RANGEINDEX:
			CurrentPlayerMode = EPlayerMode::RangeMode;
			break;
		case THROWINDEX:
			CurrentPlayerMode = EPlayerMode::ThrowMode;
			break;
	}

	if (CurrentWeapon && WeaponToSwap)
	{
		PlayerAnimInstance->Montage_Play(CurrentWeapon->Montage_PutBackWeapon);
		SetMontageDelegate(CurrentWeapon->Montage_PutBackWeapon, PlayerAnimInstance->DisarmEndDelegate, FName("DisarmWeapon"));
		bCannotAction = true;
		ResetPlayerVariables();
	}
	else
	{
		EquipWeapon();
	}
}

void AZombiePlayer::DisarmWeapon(UAnimMontage* Montage, bool bInterrupted)
{
	if (CurrentWeapon == nullptr) return;

	//AWeaponThrowBase* Throwing = Cast<AWeaponThrowBase>(CurrentWeapon);
	if (!bThrowingLaunched)
		SetActiveWeapon(CurrentWeapon, false);

	AssignWeaponToPlayer(nullptr);

	EquipWeapon();
}

void AZombiePlayer::SetNextWeaponIndex(const int32 Type)
{
	switch (Type)
	{
		case MELEEINDEX:
			RecentMeleeWeaponIndex = (RecentMeleeWeaponIndex + 1) % Num_MaxMeleeWeapon;
			break;
		case RANGEINDEX:
			RecentRangeWeaponIndex = (RecentRangeWeaponIndex + 1) % Num_MaxRangeWeapon;
			break;
		case THROWINDEX:
			int32 IndexTemp = RecentThrowingWeaponIndex;

			RecentThrowingWeaponIndex = (RecentThrowingWeaponIndex + 1) % Num_MaxThrowingTypes; // 0 or 1 
			//PlayerInventory->CheckThrowingNum((EThrowingTag)((RecentThrowingWeaponIndex + 1) % Num_MaxThrowingTypes)
			
			while (PlayerInventory->GetThrowingNum(RecentThrowingWeaponIndex) == 0)
			{
				if (RecentThrowingWeaponIndex == IndexTemp)
				{
					PlayerInventory->bNoThrowings = true;
					break;
				}
				RecentThrowingWeaponIndex = (RecentThrowingWeaponIndex + 1) % Num_MaxThrowingTypes;
			}
			break;
	}
	return;
}

void AZombiePlayer::EquipWeapon()
{
	bThrowingLaunched = false;

	if (!WeaponToSwap) return;
	else
	{
		DS(*(GetDebugName(WeaponToSwap) + " is equipped"), 3.0f);

		// Set weapon internal variable
		AssignWeaponToPlayer(WeaponToSwap);
		SetActiveWeapon(CurrentWeapon, true);

		// Attach weapon to hand
		const FTransform& PlacementTransform = CurrentWeapon->PlacementTransform * GetMesh()->GetSocketTransform(FName("weaponsocket_r"));
		CurrentWeapon->SetActorTransform(PlacementTransform, false, nullptr, ETeleportType::TeleportPhysics);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("weaponsocket_r"));

		// Animation
		PlayerAnimInstance->Montage_Play(CurrentWeapon->Montage_BringWeapon);
		SetMontageDelegate(CurrentWeapon->Montage_BringWeapon, PlayerAnimInstance->WeaponActionEndDelegate, FName("OnSwapProcessEnded"));
		
		// Delegate Broacast
		if (OnWeaponChanged.IsBound()) OnWeaponChanged.Broadcast();
	}
}

void AZombiePlayer::PutKnifeOn()
{
	SetActiveWeapon(CurrentWeapon, false);
	KnifeMesh->SetHiddenInGame(false);
}

void AZombiePlayer::PutKnifeOff()
{
	KnifeMesh->SetHiddenInGame(true);

	WeaponToSwap = CurrentWeapon;
	EquipWeapon();
}

void AZombiePlayer::LootWeapon(AWeapon* Weapon) // Weapon in Parameter means the weapon you looted just now
{
	if(Weapon==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon is not valid"));
		return;
	}
	
	EWeaponTag ItemTag = Weapon->GetItemTag();
	int32 WeaponSlot = -1;
	int8 BlankSlot = 0;

	UE_LOG(LogTemp, Warning, TEXT("LOOTWEAPON: %s"), *Weapon->GetName());
	switch (ItemTag)
	{
		case EWeaponTag::MELEE:
			BlankSlot = PlayerInventory->GetMeleeBlankSlot();
			if (BlankSlot == -1)
			{
				DropWeapon(MELEEINDEX, RecentMeleeWeaponIndex);
				PlayerInventory->Inventory_SetWeapon(MELEEINDEX, Weapon, RecentMeleeWeaponIndex);
			}
			else
			{
				PlayerInventory->Inventory_SetWeapon(MELEEINDEX, Weapon, BlankSlot);
			}
			break;

		case EWeaponTag::RANGE:
			BlankSlot = PlayerInventory->GetRangeBlankSlot();
			if (BlankSlot == -1)
			{
				DropWeapon(RANGEINDEX, RecentRangeWeaponIndex);
				PlayerInventory->Inventory_SetWeapon(RANGEINDEX, Weapon, RecentRangeWeaponIndex);
			}
			else
			{
				PlayerInventory->Inventory_SetWeapon(RANGEINDEX, Weapon, BlankSlot);
			}
			break;
		
		case EWeaponTag::THROW:
			AWeaponThrowBase* Throwing = Cast<AWeaponThrowBase>(Weapon);
			if (Throwing)
			{
				EThrowingTag ThrowingTag = Throwing->GetThrowingTag();
				if (ThrowingTag == EThrowingTag::NONE)
				{
					UE_LOG(LogTemp, Error, TEXT("LOOTWEAPON / Throwing Tag Error: Throwing tag must be set in throwing weapon bp"));
				}

				WeaponSlot = (int32)ThrowingTag - 1; // Fixed slot for each throwing types(brick, grenade, etc)
			}

			PlayerInventory->Inventory_SetWeapon(THROWINDEX, Weapon, WeaponSlot);
			
			if(PlayerInventory->bNoThrowings)
				PlayerInventory->bNoThrowings = false;

			break;
	}
	SetActiveWeapon(Weapon, false);
	DS(*(GetDebugName(Weapon) + " is looted"),5.0f);
}

void AZombiePlayer::DropWeapon(const int32 WeaponIndex, const int32 WeaponSlot)
{
	// �ڡ�TODO: Throw the weapon in inventory to the world
	
	PlayerInventory->Inventory_GetWeapon(WeaponIndex, WeaponSlot)->Player = nullptr;	
	PlayerInventory->Inventory_ClearWeapon(WeaponIndex, WeaponSlot); 

}

bool AZombiePlayer::CanAttack()
{
	return (!bCannotAction && CurrentWeapon->CanAttack() && CanAct());
}

void AZombiePlayer::AssignWeaponToPlayer(AWeapon* Weapon)
{
	if (Weapon)
	{
		CurrentWeapon = Weapon;
		CurrentWeapon->Player = this;
		CurrentWeapon->ConvertPlayerController();
		CurrentWeapon->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Controller = nullptr;
		CurrentWeapon->Player = nullptr;
		CurrentWeapon = nullptr;
	}

}

void AZombiePlayer::SetActiveWeapon(AWeapon* Weapon, bool bIsSet)
{
	Weapon->SetActorHiddenInGame(!bIsSet);
	Weapon->SetActorEnableCollision(bIsSet);
	Weapon->SetActorTickEnabled(bIsSet);
}

void AZombiePlayer::Attack()
{
	if (!CurrentWeapon) return;

	if (CanAttack())
	{
		switch (CurrentPlayerMode) // Condition for immediate return
		{
			case EPlayerMode::MeleeMode:
				if (SwingCount >= 4)
					return;
				else
					++SwingCount;
				break;
			case EPlayerMode::RangeMode:
				break;
			case EPlayerMode::ThrowMode:
				break;
		}
		CurrentWeapon->Attack();
		AZombiePlayerController* ZombiePlayerController = Cast<AZombiePlayerController>(Controller);
		CurrentLoudness = CurrentWeapon->WeaponLoudness;
		ZombiePlayerController->OnNoiseDelegate.Broadcast(CurrentLoudness, GetActorLocation(), LoudnessRange);
		//NoiseEmitter->Execute_MakeEnvironmentNoise(NoiseEmitter->_getUObject(), this, CurrentWeapon->WeaponLoudness, GetActorLocation(), CurrentWeapon->WeaponLoudnessRange);
	}
	bCanThrow = false; // true when btn released.
}

bool AZombiePlayer::IsExecutable()
{
	return false;
}

void AZombiePlayer::ResetPlayerVariables()
{
	// TODO: Implement other variables 
	bDoingAttack = false; 
	bCanThrow = false;
}

void AZombiePlayer::SetMontageDelegate(UAnimMontage* Montage, FOnMontageEnded Delegate, FName FunctionName)
{
	Delegate.BindUFunction(this, FunctionName);
	PlayerAnimInstance->Montage_SetEndDelegate(Delegate, Montage);
}

void AZombiePlayer::OnSwapProcessEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bCannotAction = false;
	PlayerAnimInstance->WeaponActionEndDelegate.Unbind();
	WeaponToSwap = nullptr;

	if (OnWeaponChanged.IsBound()) OnWeaponChanged.Broadcast();
}
