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
	const FVector TargetLocation =  ExecutedZombie->GetMesh()->GetBoneLocation("head",EBoneSpaces::WorldSpace);
	const FVector PlayerRayLocation = Player->Camera->GetComponentLocation();

	FVector ExecutionLocation = TargetLocation - UKismetMathLibrary::GetForwardVector(ExecutedZombie->GetActorRotation()) * Player->ExecutableDistance;
	ExecutionLocation.Z = Player->GetActorLocation().Z;
	Player->SetActorLocation(ExecutionLocation);
	Player->MotionWarping->AddOrUpdateWarpTargetFromLocation("ExecutionLocation", ExecutionLocation);
	
	FVector ExecutionRotation = TargetLocation;
	const FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, ExecutionRotation);
	Player->Camera->SetWorldRotation(RotationToTarget);
}

void UAnimNotifyState_Execute::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!Player) return;
	
	if(!ExecutedZombie) return;
	
	const FVector TargetLocation =  ExecutedZombie->GetMesh()->GetBoneLocation("head",EBoneSpaces::WorldSpace);
	const FVector PlayerRayLocation = Player->Camera->GetComponentLocation();
	
	/*FVector ExecutionLocation = TargetLocation - UKismetMathLibrary::GetForwardVector(ExecutedZombie->GetActorRotation()) * Player->ExecutableDistance;
	ExecutionLocation.Z = PlayerRayLocation.Z;*/

	const FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(PlayerRayLocation, TargetLocation);
	Player->Camera->SetWorldRotation(RotationToTarget);
}

void UAnimNotifyState_Execute::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!Player) return;
}
