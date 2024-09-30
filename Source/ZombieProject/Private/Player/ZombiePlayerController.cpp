// Fill out your copyright notice in the Description page of Project Settings.
#include "Player\ZombiePlayerController.h"

#include "AsyncTreeDifferences.h"
#include "Player\ZombiePlayer.h"
#include "Player\ZombiePlayerAnimInstance.h"
#include "Weapon\WeaponThrowBase.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/InteractableInterface.h"
#include "InventoryComponent.h"

#define D(X,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT(X))); // For debugging, X muse be const FString
#define DI(I,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT("%d"),I));
#define DS(S,T) GEngine->AddOnScreenDebugMessage(-1, T, FColor::Red, FString::Printf(TEXT("%s"),S)); //For debugging with variable, X and Y muse be String

void AZombiePlayerController::BeginPlay() {
	Super::BeginPlay();

	ZombiePlayer = Cast<AZombiePlayer>(GetPawn());
	check(ZombiePlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	OnNoiseDelegate.AddUFunction(this,FName("MakeEnvNoise"));
	OnNoiseDelegate.AddUFunction(this,FName("MakeNoiseEffect"));

	HUDWidget = CreateWidget(this, HUDWidgetClass);
	if (!HUDWidget->IsInViewport()) {
		HUDWidget->AddToViewport();
	}
	if(HUDWidget->Implements<UInteractionUI>())
	{
		InteractionUI = TScriptInterface<IInteractionUI>(HUDWidget);
	}
	else
	{
		check(0 && "HUDWidget does not implement IInteractionUI");
	}
}

void AZombiePlayerController::Tick(float DeltaTime)
{
	//Idle();
	//Run Idle CamShake when not move, attack, or any kind of player acition
	if (ZombiePlayer->CanAct()) {
		Idle();
	}

	// Update Interaction UI
	if(ZombiePlayer->CanAct())
	{
		if(ZombiePlayer->TracedActor && ZombiePlayer->TracedActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
		{
			IInteractableInterface* IInteractable = Cast<IInteractableInterface>(ZombiePlayer->TracedActor);
			EInteractionType NameInteractWith = IInteractable->Execute_GetInteractTag(ZombiePlayer->TracedActor);
			auto isInterctable = IInteractable->Execute_CanInteract(ZombiePlayer->TracedActor, ZombiePlayer);
			InteractionUI.GetInterface()->Execute_UpdateUI(InteractionUI.GetObject(), NameInteractWith, isInterctable);
		}
		else
		{
			InteractionUI.GetInterface()->Execute_UpdateUI(InteractionUI.GetObject(), EInteractionType::Default, false);
		}
	}
	else
	{
		InteractionUI.GetInterface()->Execute_UpdateUI(InteractionUI.GetObject(), EInteractionType::Default, false);
	}
}

void AZombiePlayerController::SetupInputComponent() {

	Super::SetupInputComponent();

	UEnhancedInputComponent* PEnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	check(PEnhancedInput);

	/*
	* Bind player movement
	*/
	PEnhancedInput->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &AZombiePlayerController::Move);
	PEnhancedInput->BindAction(InputActions->InputSprint, ETriggerEvent::Triggered, this, &AZombiePlayerController::Sprint);
	PEnhancedInput->BindAction(InputActions->InputSprint, ETriggerEvent::Completed, this, &AZombiePlayerController::EndSprint);
	PEnhancedInput->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &AZombiePlayerController::Look);
	PEnhancedInput->BindAction(InputActions->InputCrouch, ETriggerEvent::Triggered, this, &AZombiePlayerController::DoCrouch);
	PEnhancedInput->BindAction(InputActions->InputKick, ETriggerEvent::Triggered, this, &AZombiePlayerController::Kick);
	//PEnhancedInput->BindAction(InputActions->InputQTE, ETriggerEvent::Triggered, this, &AZombiePlayerController::QTE);
	

	/*
	* Bind player interaction
	*/
	PEnhancedInput->BindAction(InputActions->InputFlashlight, ETriggerEvent::Triggered, this, &AZombiePlayerController::OnOffFlashlight);
	PEnhancedInput->BindAction(InputActions->InputInteract, ETriggerEvent::Triggered, this, &AZombiePlayerController::Interact);

	/*
	* Bind player 
	*/
	PEnhancedInput->BindAction(InputActions->InputSwap, ETriggerEvent::Triggered, this, &AZombiePlayerController::PutWeapon);
	PEnhancedInput->BindAction(InputActions->InputAttack, ETriggerEvent::Triggered, this, &AZombiePlayerController::LClick);
	PEnhancedInput->BindAction(InputActions->InputSubAction, ETriggerEvent::Triggered, this, &AZombiePlayerController::RClick);
	PEnhancedInput->BindAction(InputActions->InputReload, ETriggerEvent::Triggered, this, &AZombiePlayerController::Reload);
	PEnhancedInput->BindAction(InputActions->InputAttackBtnUp, ETriggerEvent::Triggered, this, &AZombiePlayerController::ReleaseThrow);
	
	/*
	* Bind player parkour
	*/
	PEnhancedInput->BindAction(InputActions->InputParkour, ETriggerEvent::Triggered, this, &AZombiePlayerController::Parkour);
} 

void AZombiePlayerController::Idle()
{
	ZombiePlayer->RunCamShake(EPlayerCamShake::Idle);
}

/*
* ZombiePlayer Move
*/


void AZombiePlayerController::Move(const FInputActionValue& Value) {
	if (!ZombiePlayer->CanAct()) return;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Move Start!"));
	FVector2D MoveValue = Value.Get<FVector2D>();
	if (ZombiePlayer->bIsAiming || ZombiePlayer->bDoingAttack) MoveValue = MoveValue * 0.3f;

	ZombiePlayer->MovementInput = MoveValue;
	
	const FRotator MovementRotation(0, GetControlRotation().Yaw, 0);

	// Forward/Backward direction
	if (MoveValue.Y != 0.f)
	{
		// Get forward vector
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		ZombiePlayer->AddMovementInput(Direction, MoveValue.Y);
	}

	// Right/Left direction
	if (MoveValue.X != 0.f)
	{
		// Get right vector
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

		ZombiePlayer->AddMovementInput(Direction, MoveValue.X);

	}

	// if move backward, move with less speed
	if (MoveValue.Y < 0) {
		ZombiePlayer->bIsMovingBackward = true;
		if (ZombiePlayer->bOnCrouched){
			ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->BackCrouchSpeed;
		}
		else if(!ZombiePlayer->bIsSprinting){
			ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->BackWalkSpeed;
		}
	}
	else {
		ZombiePlayer->bIsMovingBackward = false;
		if (ZombiePlayer->bOnCrouched) {
			ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->CrouchSpeed;
		}
		else if(!ZombiePlayer->bIsSprinting) {
			ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->WalkSpeed;
		}
	}

	ZombiePlayer->RunCamShake(EPlayerCamShake::Walk);
	float ActualWalkLoudness = ZombiePlayer->WalkLoudness;
	if (ZombiePlayer->bOnCrouched) return; //ActualWalkLoudness = ZombiePlayer->CrouchLoudness;
	ZombiePlayer->CurrentLoudness = ActualWalkLoudness;
	OnNoiseDelegate.Broadcast(ZombiePlayer->CurrentLoudness, ZombiePlayer->GetActorLocation(), ZombiePlayer->LoudnessRange);
}

void AZombiePlayerController::Sprint() {
	if (!ZombiePlayer->CanSprint()) {
		if(ZombiePlayer->bIsSprinting)
		{
			EndSprint();
		}
		return;
	}
	ZombiePlayer->bCanRecoverStamina = false;

	if (ZombiePlayer->bIsAiming) ZombiePlayer->bIsAiming = false;

	ZombiePlayer->bIsSprinting = true;
	//D("Player Sprint invoked", 1.0f);
	ZombiePlayer->RunCamShake(EPlayerCamShake::Run);
	if (ZombiePlayer->bIsMovingBackward) {
		ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->BackSprintSpeed;
	}
	else {
		ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->SprintSpeed;
	}
	ZombiePlayer->Stamina -= ZombiePlayer->SprintStaminaAmount;
	OnNoiseDelegate.Broadcast(ZombiePlayer->CurrentLoudness, ZombiePlayer->GetActorLocation(), ZombiePlayer->LoudnessRange);
}

void AZombiePlayerController::EndSprint() {
	//D("Player EndSprint invoked", 1.0f);
	ZombiePlayer->bIsSprinting = false;
	ZombiePlayer->GetCharacterMovement()->MaxWalkSpeed = ZombiePlayer->WalkSpeed;
	ZombiePlayer->WaitToRecoverStamina();
}

void AZombiePlayerController::Look(const FInputActionValue& Value) {
	if (!ZombiePlayer->CanAct()) return;
	if(!ZombiePlayer->Camera->bUsePawnControlRotation) return;
	
	FVector2D LookValue = Value.Get<FVector2D>();

	float MaxSense = ZombiePlayer->MaxCameraLookSensitivity;

	// Restrict mouse sensitivity
	//LookValue.X = FMath::Clamp(LookValue.X, -MaxSense, MaxSense);
	//LookValue.Y = FMath::Clamp(LookValue.Y, -MaxSense, MaxSense);
	
	//UE_LOG(LogTemp, Warning, TEXT("LookValue %f %f"), LookValue.X, LookValue.Y);
	//FTimerHandle XTimer;
	//GetWorld()->GetTimerManager().SetTimer(XTimer, [&]()
	//{
	//	if (LookValue.X != 0.f)
	//	{
	//		ZombiePlayer->AddControllerYawInput(LookValue.X * ZombiePlayer->CameraSensitivity);
	//	}
	//	GetWorld()->GetTimerManager().ClearTimer(XTimer);
	//}, 0.01f, false);

	//FTimerHandle YTimer;
	//GetWorld()->GetTimerManager().SetTimer(YTimer, [&]()
	//	{
	//		if (LookValue.Y != 0.f)
	//		{
	//			ZombiePlayer->AddControllerPitchInput(-LookValue.Y * ZombiePlayer->CameraSensitivity);
	//		}
	//		GetWorld()->GetTimerManager().ClearTimer(YTimer);
	//	}, 0.01f, false);
	if (LookValue.X != 0.f)
	{
		ZombiePlayer->AddControllerYawInput(LookValue.X * ZombiePlayer->CameraSensitivity);
		// Rotate Player's arm
		//ZombiePlayer->GetMesh()->Transform
	}
	if (LookValue.Y != 0.f)
	{
		ZombiePlayer->AddControllerPitchInput(-LookValue.Y * ZombiePlayer->CameraSensitivity);
	}
	//ZombiePlayer->SetActorRotation(ZombiePlayer->Camera->GetComponentQuat());
	
	// Set Timer to get mesh follow camera rotation
	//if (GetWorld()->GetTimerManager().GetTimerRemaining(ZombiePlayer->FollowRotationTimer) <= 0) {
	//	GetWorld()->GetTimerManager().SetTimer(ZombiePlayer->FollowRotationTimer, [&]()
	//	{
	//		if (ZombiePlayer->CanAct()) {
	//			ZombiePlayer->SetActorRotation(ZombiePlayer->Camera->GetComponentQuat());
	//		}
	//		GetWorld()->GetTimerManager().ClearTimer(ZombiePlayer->FollowRotationTimer);
	//	}, 0.01f, false);
	//}
}

void AZombiePlayerController::DoCrouch_Implementation() {
	if (!ZombiePlayer->CanAct()) return;
}

void AZombiePlayerController::OnOffFlashlight_Implementation() {
	if (!ZombiePlayer->CanAct()) return;
	/*UE_LOG(LogTemp, Warning, TEXT("On Off Flash Light"));
	if (const float LightIntensity = ZombiePlayer->Flashlight->Intensity > 0) ZombiePlayer->Flashlight->SetIntensity(0);
	else ZombiePlayer->Flashlight->SetIntensity(ZombiePlayer->FlashlightIntensity);*/
}

void AZombiePlayerController::Kick()
{
	if (!ZombiePlayer->CanKick()) return;
	if (ZombiePlayer->KickMontage == nullptr) {
		D("KickMontage not found", 1.0f);
		return;
	}
	ZombiePlayer->bCannotAction = true;
	ZombiePlayer->PlayerAnimInstance->Montage_Play(ZombiePlayer->KickMontage);
	D("Player Kick invoked", 1.0f);
	OnNoiseDelegate.Broadcast(ZombiePlayer->CurrentLoudness, ZombiePlayer->GetActorLocation(), ZombiePlayer->LoudnessRange);
}

/*
* ZombiePlayer Interact
*/

void AZombiePlayerController::Interact() {
	if (ZombiePlayer->IsInteractable()) {
		// Do Interact
		EInteractionType NameInteractWith = IInteractableInterface::Execute_GetInteractTag(ZombiePlayer->TracedActor);
		FString Name = UEnum::GetValueAsString( NameInteractWith );
		DS(*Name, 1.0f);
		
		if (IInteractableInterface::Execute_CanInteract(ZombiePlayer->TracedActor, ZombiePlayer)) {
			switch (NameInteractWith)
			{
			case EInteractionType::Open:
				break;
			case EInteractionType::Pickup:
				break;
			case EInteractionType::Assassinate:
				break;
			default:
				break;
			}
			IInteractableInterface::Execute_PerformInteract(ZombiePlayer->TracedActor, ZombiePlayer);
		}
	}
}
/*
 *bp���� ó��
void AZombiePlayerController::QTE()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QTE"));
	UE_LOG(LogTemp, Warning, TEXT("QTE!"));
	if(!ZombiePlayer->bCanQTE) //QTE�� ������ ��Ȳ
	{
		if(ZombiePlayer->QTECount <= 0)
		{
			ZombiePlayer->EscapeQTE();
		}
		else
		{
			ZombiePlayer->QTECount--;
		}
	}
}
*/
/*
* ZombiePlayer Parkour
*/

void AZombiePlayerController::Parkour() {
	if (!ZombiePlayer->CanAct()) return;
	if(!ZombiePlayer->GetCharacterMovement()->IsMovingOnGround()) return;
	bool bCanParkour = false;
	EDrawDebugTrace::Type DebugTraceType = EDrawDebugTrace::None;
	if(ZombiePlayer->bDrawParkourDebug){
		DebugTraceType = EDrawDebugTrace::ForDuration;
	}
	
	// Get a Player Height by line tracing
	float PlayerHeight = 0;
	FVector PlayerStartLocation = ZombiePlayer->Camera->GetComponentLocation();
	FVector PlayerEndLocation = PlayerStartLocation - FVector(0,0,20000);
	if(FHitResult hit; UKismetSystemLibrary::LineTraceSingle(GetWorld(),PlayerStartLocation,PlayerEndLocation,ETraceTypeQuery::TraceTypeQuery1,
			false, {ZombiePlayer->CurrentWeapon, ZombiePlayer}, DebugTraceType, hit, true))
	{
		PlayerHeight = PlayerStartLocation.Z - hit.Location.Z;
	}
	
	ZombiePlayer->ParkourEndLocation = FVector(0,0,20000);
	
	float ParkourHeight = 0, ParkourWidth = 0;

	// 0: Vaulting, 1: Climbing
	enum EParkourType
	{
		Vaulting,
		Climbing,
		None,
	};
	EParkourType ParkourType = None;
	
	bool bHitStartRay = false;
	bool bHitMiddleRay = false ;
	bool bHitEndRay = false;
	FHitResult StartHit;
	constexpr int HeightNum = 30;
	for(int i=0; i<HeightNum; i++){
		FVector StartLocation = PlayerStartLocation;
		StartLocation.Z += PlayerHeight;
		StartLocation.Z -= PlayerHeight*2/HeightNum * i;
		FVector EndLocation = StartLocation + UKismetMathLibrary::GetForwardVector(ZombiePlayer->GetActorRotation()) * ZombiePlayer->ParkourDistance;
		if(UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery1,
			false, {ZombiePlayer->CurrentWeapon, ZombiePlayer}, DebugTraceType, StartHit, true))
		{
			//ParkourHeight = PlayerHeight - (ZombiePlayer->Camera->GetComponentLocation().Z - StartLocation.Z);
			bHitStartRay = true;
			break;
		}
	}

	if(!bHitStartRay) return;

	// Check Parkourable Tag
	if(bHitStartRay && !StartHit.GetActor()->ActorHasTag("Parkour"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor does not have Parkour tag"));
		return;
	}
	
	FHitResult MiddleHit;
	constexpr int WidthNum = 30;
	for(int i=0; i<WidthNum; i++)
	{
		FVector StartLocation = StartHit.Location;
		StartLocation.Z += PlayerHeight;
		StartLocation += (ZombiePlayer->MaxParkourWidth/WidthNum * i) * UKismetMathLibrary::GetForwardVector(ZombiePlayer->GetActorRotation());
		FVector EndLocation = StartLocation - FVector(0,0,PlayerHeight);
		if(UKismetSystemLibrary::SphereTraceSingle(_getUObject(),StartLocation,EndLocation,5,ETraceTypeQuery::TraceTypeQuery1,
			false, {ZombiePlayer->CurrentWeapon, ZombiePlayer}, DebugTraceType, MiddleHit, true))
		{
			if(i==0)
			{
				ZombiePlayer->ParkourStartLocation = MiddleHit.Location;
			}
			ZombiePlayer->ParkourMiddleLocation = MiddleHit.Location;
			bHitMiddleRay = true;
		}
		else
		{
			ParkourWidth = (ZombiePlayer->ParkourStartLocation - ZombiePlayer->ParkourMiddleLocation).Size();
			FHitResult EndHit;
			FVector NewStartLocation = MiddleHit.TraceStart + UKismetMathLibrary::GetForwardVector(ZombiePlayer->GetActorRotation()) * ZombiePlayer->ParkourDistance;
			FVector NewEndLocation = NewStartLocation - FVector(0,0,10000);
			if(UKismetSystemLibrary::SphereTraceSingle(_getUObject(),NewStartLocation,NewEndLocation,5,ETraceTypeQuery::TraceTypeQuery1,
			false, {ZombiePlayer->CurrentWeapon, ZombiePlayer}, DebugTraceType, EndHit, true,FLinearColor::Blue))
			{
				ZombiePlayer->ParkourEndLocation = EndHit.Location;
				bHitEndRay = true;
				break;
			}
		}
	}

	if(!bHitMiddleRay) return;
	
	bCanParkour = (bHitStartRay && bHitMiddleRay);

	// Calculate ParkourHeight
	if(ZombiePlayer->ParkourStartLocation.Z > ZombiePlayer->Camera->GetComponentLocation().Z)
	{
		ParkourHeight = PlayerHeight + (ZombiePlayer->ParkourStartLocation.Z - ZombiePlayer->Camera->GetComponentLocation().Z);
	}
	else
	{
		ParkourHeight = PlayerHeight - (ZombiePlayer->Camera->GetComponentLocation().Z - ZombiePlayer->ParkourStartLocation.Z);
	}
	
	if(ZombiePlayer->MaxParkourHeights.Num() < EParkourType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Parkour Heights num is less than EParkourType size"));
		return;
	}

	if(ZombiePlayer->MaxParkourHeights[Vaulting] <= ParkourHeight && ParkourHeight <= ZombiePlayer->MaxParkourHeights[Climbing])
	{
		bCanParkour = true;
		ParkourType = Climbing;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(ZombiePlayer->ParkourEndLocation.Z,
			ZombiePlayer->GetMesh()->GetComponentLocation().Z - ZombiePlayer->MaxParkourHeights[Vaulting],
			ZombiePlayer->GetMesh()->GetComponentLocation().Z + ZombiePlayer->MaxParkourHeights[Vaulting]) &&
			ZombiePlayer->MaxParkourHeights[Vaulting] >= ParkourHeight)
	{
		bCanParkour = true;
		ParkourType = Vaulting;
	}

	if (!bCanParkour || ParkourType == None) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Player Start Location: %f, %f, %f"), PlayerStartLocation.X, PlayerStartLocation.Y, PlayerStartLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("Parkour Height, Width: %f, %f"), ParkourHeight, ParkourWidth);
	UE_LOG(LogTemp, Warning, TEXT("Max Height, WIdth: %f, %f"), ZombiePlayer->MaxParkourHeights[ParkourType], ZombiePlayer->MaxParkourWidth);

	FVector PlayerParkourStartLocation = ZombiePlayer->ParkourStartLocation;
	PlayerParkourStartLocation.X -= UKismetMathLibrary::GetForwardVector(ZombiePlayer->Camera->GetComponentRotation()).X * ZombiePlayer->ParkourDistance;
	PlayerParkourStartLocation.Y -= UKismetMathLibrary::GetForwardVector(ZombiePlayer->Camera->GetComponentRotation()).Y * ZombiePlayer->ParkourDistance;
	ZombiePlayer->SetActorLocation(PlayerParkourStartLocation);
	
	DrawDebugSphere(GetWorld(),ZombiePlayer->ParkourStartLocation,10,12,FColor::Magenta,false,10.0f);
	DrawDebugSphere(GetWorld(),ZombiePlayer->ParkourMiddleLocation,10,12,FColor::Yellow, false, 10.0f);
	DrawDebugSphere(GetWorld(),ZombiePlayer->ParkourEndLocation,10,12,FColor::Emerald, false, 10.0f);
	ZombiePlayer->MotionWarping->AddOrUpdateWarpTargetFromLocation("ParkourStart",ZombiePlayer->ParkourStartLocation);
	ZombiePlayer->MotionWarping->AddOrUpdateWarpTargetFromLocation("ParkourMiddle",ZombiePlayer->ParkourMiddleLocation);
	ZombiePlayer->MotionWarping->AddOrUpdateWarpTargetFromLocation("ParkourEnd",ZombiePlayer->ParkourEndLocation);
	
	// Play Parkour Animation
	if (ZombiePlayer->ParkourMontages.IsValidIndex(ParkourType) && !ZombiePlayer->ParkourMontages[ParkourType]) return;
	UE_LOG(LogTemp, Warning, TEXT("Start Parkour with parkour type: %d"), ParkourType);
	ZombiePlayer->PlayerAnimInstance->Montage_Play(ZombiePlayer->ParkourMontages[ParkourType], 1.0f);
	ZombiePlayer->CurrentLoudness = ZombiePlayer->ParkourLoudness;
	//OnNoiseDelegate.Broadcast();
}

/*
* Player Attack
*/

void AZombiePlayerController::PutWeapon(const FInputActionValue& Value)
{
	const FVector SwapValue = Value.Get<FVector>();

	if (!ZombiePlayer->bCannotAction && !ZombiePlayer->bDoingAttack)
	{
		if (SwapValue.X != 0.f) // 1 Pressed
		{
			if (ZombiePlayer->CurrentPlayerMode == EPlayerMode::MeleeMode) return;

			ZombiePlayer->WeaponChange(MELEEINDEX, ZombiePlayer->RecentMeleeWeaponIndex);
		}
		//else if (SwapValue.Y != 0.f) // 2 Pressed
		//{
		//	if (ZombiePlayer->CurrentPlayerMode == EPlayerMode::RangeMode)
		//		ZombiePlayer->SetNextWeaponIndex(RANGEINDEX);

		//	ZombiePlayer->WeaponChange(RANGEINDEX, ZombiePlayer->RecentRangeWeaponIndex);
		//}
		else if (SwapValue.Z != 0.f) // 3 Pressed
		{
			if (ZombiePlayer->CurrentPlayerMode == EPlayerMode::ThrowMode)
				ZombiePlayer->SetNextWeaponIndex(THROWINDEX);

			ZombiePlayer->WeaponChange(THROWINDEX, ZombiePlayer->RecentThrowingWeaponIndex);
		}
		DI((int32)ZombiePlayer->CurrentPlayerMode, 3.0f);
	}
	
}

void AZombiePlayerController::LClick()
{
	// Exception for throw mode
	if (ZombiePlayer->CurrentPlayerMode == EPlayerMode::ThrowMode)
	{
		ZombiePlayer->SetPlayerThrowFlag(false);
	}

	ZombiePlayer->Attack();
}

void AZombiePlayerController::RClick()
{
	if (ZombiePlayer->CurrentWeapon == nullptr) return;
	ZombiePlayer->CurrentWeapon->SubAction(); // Blank function in MeleeBase
}

void AZombiePlayerController::ReleaseThrow()
{
	if (ZombiePlayer->CurrentWeapon && ZombiePlayer->CurrentPlayerMode == EPlayerMode::ThrowMode)
	{
		ZombiePlayer->SetPlayerThrowFlag(true);
		D("THROW RELEASED RELEASED ", 1.0f);
	}
}

void AZombiePlayerController::Reload() // When pressing R
{
	D("ZombiePlayer Reload invoked",1.0f);
	if (ZombiePlayer->CurrentWeapon == nullptr) return;
	AWeaponGunBase* CurrentGun = Cast<AWeaponGunBase>(ZombiePlayer->CurrentWeapon);

	if (CurrentGun)
	{
		CurrentGun->Reload();
	}
}

void AZombiePlayerController::MakeEnvNoise(float Loudness, const FVector& NoiseLocation, float Range)
{
	if(!ZombiePlayer->NoiseEmitter) return;
	ZombiePlayer->NoiseEmitter->Execute_MakeEnvironmentNoise(ZombiePlayer->NoiseEmitter, ZombiePlayer, Loudness, NoiseLocation, Range);
	ZombiePlayer->CurrentLoudness = 0;
}

void AZombiePlayerController::MakeNoiseEffect(float Loudness, const FVector& NoiseLocation, float Range)
{
	// For Debug
	if(ZombiePlayer->bDrawNoiseDebug)
	{
		DrawDebugSphere(GetWorld(),NoiseLocation,Range,12, FColor::Red, false, 1.0f);
	}
}

