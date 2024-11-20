// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify\AnimNotify_EndParkour.h"
#include "Player\ZombiePlayer.h"

void UAnimNotify_EndParkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	Player = Cast<AZombiePlayer>(MeshComp->GetOwner());
	if (!Player) return;
	
	//Player->Camera->AttachToComponent(Player->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);Player->Camera->SetRelativeLocation(FVector(-7,0,71));
	//Player->GetMesh()->AttachToComponent(Player->Camera,FAttachmentTransformRules::KeepRelativeTransform);
	
	//Player->bOnParkour = false;
	//Player->Camera->bUsePawnControlRotation = true;
	//Player->Controller->SetControlRotation(Player->Camera->GetComponentRotation());
	//Player->ParkourEndLocation = FVector(0,0,20000);
}
