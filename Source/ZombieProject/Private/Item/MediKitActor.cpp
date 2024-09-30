// Fill out your copyright notice in the Description page of Project Settings.
#include "Item/MediKitActor.h"

// Sets default values
// Constructor 에서 Event 를 Bind 하면 안됨, BeginPlay 에서 Bind 해야함
AMediKitActor::AMediKitActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the (root) component for rendering
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	//Create collision box
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	//Initialize the item data
	ItemData = FPickableObjectData(true, ItemId, Name, 1);
}

// Called when the game starts or when spawned
void AMediKitActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMediKitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMediKitActor::CanInteract_Implementation(AActor* Interactor) const
{
		return true;
}

FPickableObjectData AMediKitActor::GetItemData_Implementation()
{
	DestroySelf();
	return ItemData;
}

