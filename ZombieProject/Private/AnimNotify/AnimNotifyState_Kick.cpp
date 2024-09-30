// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotifyState_Kick.h"
#include "Player\ZombiePlayer.h"
#include "Enemy/ZombieBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_Kick::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	//D("Kick NotifyBegin invoked", 1.0f);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	bHasHit = false;
	//Player->bCannotAction = true;
	if (bHasHit) return;
    	
    FHitResult HitResult;
    
    bHasHit = UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation("KickStart"), MeshComp->GetSocketLocation("KickEnd"),
    	20.f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn), false, {Player, Player->CurrentWeapon}, EDrawDebugTrace::ForDuration, HitResult, true);
    
    AActor* HitActor = HitResult.GetActor();
    class AZombieBase* HitZombie = Cast<AZombieBase>(HitActor);
    if (!HitZombie) return;
    
    UGameplayStatics::ApplyDamage(HitZombie, Player->KickDamage, Player->GetController(), Player, nullptr);
    HitZombie->Parried();
    D("Zombie Hit", 1.0f);

	// Run Kick CamShake
	Player->RunCamShake(EPlayerCamShake::Kick);
}

void UAnimNotifyState_Kick::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	//D("Kick NotifyTick invoked", 1.0f);
	if (!Player) return;
}

void UAnimNotifyState_Kick::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	//D("Kick NotifyEnd invoked", 1.0f);
	if (!Player) return;

	Player->bCannotAction = false;
}
