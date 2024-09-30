// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PickableActor.h"


// Sets default values
APickableActor::APickableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APickableActor::CanInteract_Implementation(AActor* Interactor) const
{
	return true;
};

void APickableActor::PerformInteract_Implementation(AActor* Interactor)
{
	// 왜 실행이 안되지????
	//DisplayInteractWidget은 되는 거로 봐서 Interface를 구현하는 경우 다르게 동작하는 것 같음
	UE_LOG(LogTemp, Display, TEXT("PickableActor: PerformInteract_Implementation"))
	//Call EventDispatcher
	//if (OnPickedUpVoidDelegate.IsBound() == true)
	//	OnPickedUpVoidDelegate.Execute();

	// Destroy self at the End of the interaction
	DestroySelf(); // TODO : is this OK? Does PickableActor always return data before being destroyed?
	return;
}

void APickableActor::DisplayInteractWidget_Implementation(AActor* Interactor)
{
	UE_LOG(LogTemp, Display, TEXT("PickableActor: DisplayInteractWidget_Implementation"))
	//DestroySelf(); // TODO : is this OK? Does PickableActor always return data before being destroyed?
	return;
}


EInteractionType APickableActor::GetInteractTag_Implementation() const
{
	return InteractTag;
}

void APickableActor::DestroySelf()
{
	bool bIsBound = OnPickedUpVoidDelegate.ExecuteIfBound(ItemData); // same as above
	Destroy(); // Destroy Self
}

FPickableObjectData APickableActor::GetItemData_Implementation() {
	return ItemData;
}

void APickableActor::SetItemData_Implementation(bool bIsEnabled, int32 Id, FName Name, int32 Amount)
{
	ItemData.SetIsEnabled(bIsEnabled);
	ItemData.SetId(Id);
	ItemData.SetItemName(Name);
	ItemData.SetAmount(Amount);
}

//FPickableObjectData Getter
bool APickableActor::GetIsEnabled_Implementation() const { return ItemData.GetIsEnabled(); };
int32 APickableActor::GetId_Implementation() const { return ItemData.GetId(); };
FName APickableActor::GetItemName_Implementation() const { return ItemData.GetItemName(); };
int32 APickableActor::GetAmount_Implementation() const { return ItemData.GetAmount(); };

//FPickableObjectData Setter
void APickableActor::SetIsEnabled_Implementation(bool bNewIsEnabled) { this->ItemData.SetIsEnabled(bNewIsEnabled); };
void APickableActor::SetId_Implementation(int32 NewId) { this->ItemData.SetId(NewId); };
void APickableActor::SetItemName_Implementation(FName NewName) { this->ItemData.SetItemName(NewName); };
void APickableActor::SetAmount_Implementation(int32 NewAmount) { this->ItemData.SetAmount(NewAmount); };


