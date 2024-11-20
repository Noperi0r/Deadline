// Fill out your copyright notice in the Description page of Project Settings.

#include "Player\ZombiePlayerAnimInstance.h"
#include "Player\ZombiePlayer.h"
#include "Animation\AnimInstance.h"
#include "GameFramework\Pawn.h"

//void UZombiePlayerAnimInstance::NativeInitializeAnimation()
//{
//	Player = Cast<AZombiePlayer>(TryGetPawnOwner()); 
//	//if (!Player)
//	//{
//	//	UE_LOG(LogTemp, Warning, TEXT("ERROR AnimInstance Player cast"));
//	//}
//}

//void UZombiePlayerAnimInstance::UpdateProperties() // Needs to be called by event blueprint update animation in Blueprint
//{
//	if (!Player) Player = Cast<AZombiePlayer>(TryGetPawnOwner());
//	
//	if (Player)
//	{
//		FVector Velocity = Player->GetVelocity();
//		MovementSpeed = Velocity.Size2D();
//
//		// Implement: Set bIsAir 
//	}
//}

void UZombiePlayerAnimInstance::NativeBeginPlay()
{
	Player = Cast<AZombiePlayer>(TryGetPawnOwner());
	checkf(Player, TEXT("NO PLAYER FOUND IN ANIMINSTANCE INITIALIZATION"));
	
	OnAssinationEnded.BindUFunction(this, FName("AssasinationEndHandler"));
}

void UZombiePlayerAnimInstance::AssasinationEndHandler()
{
	Player->PutKnifeOff();
}
