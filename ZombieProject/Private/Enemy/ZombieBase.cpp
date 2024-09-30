// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ZombieBase.h"
#include "Enemy/ZombieBaseController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AZombieBase::AZombieBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Threshold = 100.0f;
	TrunThreshold = 50.0f;
	QTECount = 20;

	AZombieBase::OnDead.AddDynamic(this, &AZombieBase::DebugDele);
}

// Called when the game starts or when spawned
void AZombieBase::BeginPlay()
{
	Super::BeginPlay();

	uuid = GetUniqueID();
}

// Called every frame
void AZombieBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AZombieBase::TriggerOnDeath()
{
	auto name = this->GetFName();

	OnDead.Broadcast(FZombieData{this->GetFName(),false});
}

void AZombieBase::DebugDele(FZombieData deadZombieData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("OnDead TestDeleFunc Execute: " + deadZombieData.Name.ToString()));
}

void AZombieBase::TargetToPlayer()
{
	AZombieBaseController* ZombieController = Cast<AZombieBaseController>(GetController());
	ZombieController->SetTargetPlayer();
}

void AZombieBase::Parried_Implementation()
{

}

void AZombieBase::Turn(FVector3d NoiseLoc)
{
	FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NoiseLoc);
	//SetActorRotation(UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), NewRot, GetWorld()->GetDeltaSeconds(), 2));
	//UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), NewRot, GetWorld()->GetDeltaSeconds(), 2);
	//FaceRotation(FMath::RInterpConstantTo(GetActorRotation(), NewRot, GetWorld()->GetDeltaSeconds(), 10));
}


