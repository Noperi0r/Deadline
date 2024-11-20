// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotifyState_Parkour.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_Parkour::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	if(!Player->GetCharacterMovement()->IsValidLowLevel()) return;
	if(!Player->Controller->IsValidLowLevel()) return;
	if(!Player->Camera->IsValidLowLevel()) return;
	
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player->SetActorEnableCollision(false);
	if(Player->CurrentWeapon->IsValidLowLevel())
	{
		Player->CurrentWeapon->SetActorEnableCollision(false);
	}
	
	FRotator PlayerCamRotation = Player->Camera->GetRelativeRotation();
	PlayerCamRotation.Yaw = 90;
	Player->Camera->SetRelativeRotation(PlayerCamRotation);
	PlayerCamForward = Player->Camera->GetComponentRotation();
	if(Player->ParkourType==EParkourType::Vaulting)
	{
		PrevCamLocation = Player->Camera->GetRelativeLocation();
		//Player->GetMesh()->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		Player->Camera->AttachToComponent(Player->GetCapsuleComponent(),  FAttachmentTransformRules::KeepWorldTransform);
		Player->Camera->SetRelativeLocation(FVector(35,-1.5,85));
		//Player->Camera->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,"head");
	}
	
	/*const FVector PlayerLocation = Player->Camera->GetComponentLocation();
	FVector TargetLocation = Player->ParkourActor->GetActorLocation();
	TargetLocation.Z = PlayerLocation.Z;
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);*/

	/*const FVector PlayerCamLocation = Player->Camera->GetComponentLocation();
	const FVector PlayerForwardLocation = PlayerCamLocation + UKismetMathLibrary::GetForwardVector(Player->GetActorRotation());
	const FRotator ForwardRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCamLocation, PlayerForwardLocation);
	Player->Camera->SetWorldRotation(ForwardRotation);*/
	
	/*FRotator PlayerCamLocalForward = Player->Camera->GetRelativeRotation();
	PlayerCamLocalForward.Yaw = 90;
	Player->Camera->SetRelativeRotation(PlayerCamLocalForward);*/

	//PlayerCamForward = Player->Camera->GetComponentRotation();

	/*const FVector PlayerCamLocation = Player->Camera->GetComponentLocation();
	FVector TargetLocation = Player->ParkourEndLocation;
	TargetLocation.Z = PlayerCamLocation.Z;
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCamLocation, TargetLocation);
	Player->Camera->SetWorldRotation(NewRotation);*/
	//Player->Controller->SetControlRotation(Player->Camera->GetComponentRotation());
	Player->Camera->bUsePawnControlRotation = false;
	Player->bOnParkour = true;
}

void UAnimNotifyState_Parkour::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!Player) return;

	Player->Camera->SetWorldRotation(PlayerCamForward);
	//const FVector PlayerLocation = Player->Camera->GetComponentLocation();
	//const FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(Player->Camera->GetComponentRotation());
	/*FVector TargetLocation = Player->ParkourStartLocation;
	TargetLocation.Z = PlayerLocation.Z;*/
	//const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, CameraForwardVector);
	//Player->Camera->SetWorldRotation(NewRotation);
}

void UAnimNotifyState_Parkour::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!Player->IsValidLowLevel()) return;
	if(!Player->GetCharacterMovement()->IsValidLowLevel()) return;
	if(!Player->Controller->IsValidLowLevel()) return;
	if(!Player->Camera->IsValidLowLevel()) return;
	
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player->SetActorEnableCollision(true);

	if(Player->CurrentWeapon->IsValidLowLevel())
	{
		Player->CurrentWeapon->SetActorEnableCollision(true);
	}

	if(Player->ParkourType==EParkourType::Vaulting)
	{
		Player->Camera->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,"head");
		Player->Camera->SetRelativeLocation(PrevCamLocation);
	}

	Player->bOnParkour = false;
	Player->Camera->bUsePawnControlRotation = true;
	//Player->Controller->SetControlRotation(Player->Camera->GetComponentRotation());
	Player->ParkourEndLocation = FVector(0,0,20000);
}


