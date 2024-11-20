// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_Execute.h"
#include "Player\ZombiePlayer.h"
#include "Enemy\ZombieBase.h"

void UAnimNotifyState_Execute::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	ExecutedZombie = Cast<AZombieBase>(Player->TracedActor);
	const FVector TargetLocation =  ExecutedZombie->GetActorLocation();
	//const FVector PlayerRayLocation = Player->Camera->GetComponentLocation();
	const FVector PlayerLocation = Player->GetActorLocation();
	//TargetLocation.Z = PlayerLocation.Z;

	FVector ExecutionLocation = TargetLocation - UKismetMathLibrary::GetForwardVector(Player->GetActorRotation()) * Player->ExecutableDistance;
	ExecutionLocation.Z = PlayerLocation.Z;
	Player->SetActorLocation(ExecutionLocation);
	
	//Player->MotionWarping->AddOrUpdateWarpTargetFromLocation("ExecutionLocation", TargetLocation);
	
	Player->Camera->bUsePawnControlRotation = false;

	PrevCamRotation = Player->Camera->GetComponentRotation();
	//const FVector ExecutionRotation = TargetLocation;
	//const FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, ExecutionRotation);
	//Player->Camera->SetWorldRotation(RotationToTarget);
	//Player->Controller->SetControlRotation(RotationToTarget);
}

void UAnimNotifyState_Execute::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!Player) return;
	
	if(!ExecutedZombie) return;
		
	if(!Player->bOnExecute)
	{
		constexpr float InterpSpeed = 3.0f;
		const FRotator PlayerRayRotation = Player->Camera->GetComponentRotation();
		const FRotator NewRotation = FMath::RInterpTo(PlayerRayRotation,PrevCamRotation,FrameDeltaTime,InterpSpeed);
		Player->Camera->SetWorldRotation(NewRotation);
	}
	else
	{
		const FVector TargetLocation =  ExecutedZombie->GetMesh()->GetBoneLocation("head",EBoneSpaces::WorldSpace);
		const FVector PlayerRayLocation = Player->Camera->GetComponentLocation();
		//TargetLocation.Z = PlayerRayLocation.Z;
		const FRotator CamRotationToTarget = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, TargetLocation);
		Player->Camera->SetWorldRotation(CamRotationToTarget);
	}
}

void UAnimNotifyState_Execute::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!Player) return;
	
	Player->Camera->bUsePawnControlRotation = true;
	Player->Controller->SetControlRotation(PrevCamRotation);
}
