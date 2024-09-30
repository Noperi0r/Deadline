// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotifyState_Parkour.h"
#include "Player\ZombiePlayer.h"

void UAnimNotifyState_Parkour::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player->SetActorEnableCollision(false);
	Player->CurrentWeapon->SetActorEnableCollision(false);
	
	//Player->GetMesh()->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//Player->Camera->AttachToComponent(Player->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,"head");
	
	/*const FVector PlayerLocation = Player->Camera->GetComponentLocation();
	FVector TargetLocation = Player->ParkourActor->GetActorLocation();
	TargetLocation.Z = PlayerLocation.Z;
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);*/
	const FVector PlayerLocation = Player->Camera->GetComponentLocation();
	FVector TargetLocation = Player->ParkourStartLocation;
	TargetLocation.Z = PlayerLocation.Z;
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
	//Player->Camera->SetWorldRotation(NewRotation);
	
	Player->Camera->bUsePawnControlRotation = false;
	Player->bOnParkour = true;
}

void UAnimNotifyState_Parkour::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!Player) return;

	/*const FVector PlayerLocation = Player->Camera->GetComponentLocation();
	const FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(Player->Camera->GetComponentRotation());
	FVector TargetLocation = Player->ParkourStartLocation;
	TargetLocation.Z = PlayerLocation.Z;
	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, CameraForwardVector);
	Player->Camera->SetWorldRotation(NewRotation);*/
}

void UAnimNotifyState_Parkour::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!Player->IsValidLowLevel()) return;
	if(!Player->GetCharacterMovement()->IsValidLowLevel()) return;
	
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	Player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player->SetActorEnableCollision(true);
	Player->CurrentWeapon->SetActorEnableCollision(true);
	//Player->Camera->bUsePawnControlRotation = true;
}


