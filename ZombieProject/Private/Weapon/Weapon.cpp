// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon\Weapon.h"
#include "Kismet\GameplayStatics.h"
#include "Components\AudioComponent.h"
#include "Noise\BaseNoiseEmitterComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	AttackSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Attack Sound"));
	AttackSound->bAutoActivate = false;

	Montage_PutBackWeapon = CreateDefaultSubobject<UAnimMontage>(TEXT("Put back weapon Montage"));
	Montage_Attack = CreateDefaultSubobject<UAnimMontage>(TEXT("Attack Montage"));
	Montage_BringWeapon = CreateDefaultSubobject<UAnimMontage>(TEXT("BringWeapon Montage"));

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	NoiseEmitter = CreateDefaultSubobject<UBaseNoiseEmitterComponent>(TEXT("Weapon Noise Emitter"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

EWeaponTag AWeapon::GetItemTag() { return ItemTag; }
int32 AWeapon::GetItemCode() { return ItemCode; }

void AWeapon::SetItemTag(EWeaponTag Tag) { ItemTag = Tag; }
void AWeapon::SetItemCode(const int32 Code) { ItemCode = Code; }
void AWeapon::SetPlayer(AZombiePlayer* ZombiePlayer) { Player = ZombiePlayer; }
