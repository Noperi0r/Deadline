// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interaction/InteractableInterface.h"
#include "Kismet/GameplayStatics.h"

#include "PickableActor.generated.h"


// Serialization Example
USTRUCT(BlueprintType, Atomic)
struct FPickableObjectData
{
	GENERATED_USTRUCT_BODY()

public:
	FPickableObjectData() : Id(0), bIsEnabled(true), Name(TEXT("")), Amount(0) {};
	FPickableObjectData(bool bIsEnabled, int32 Id, FName Name, int32 Amount) : Id(Id), bIsEnabled(bIsEnabled), Name(Name), Amount(Amount) {};

	bool GetIsEnabled() const { return bIsEnabled; };
	int32 GetId() const { return Id; };
	FName GetItemName() const { return Name; };
	int32 GetAmount() const { return Amount; };

	void SetIsEnabled(bool bNewIsEnabled) { this->bIsEnabled = bNewIsEnabled; };
	void SetId(int32 NewId) { this->Id = NewId; };
	void SetItemName(FName NewName) { this->Name = NewName; };
	void SetAmount(int32 NewAmount) { this->Amount = NewAmount; };

public:
	// Item Code
	// 100 ~ 199 : ???? 200 ~ 299 : ??? ??????
	// 101 : ?? 102 : ????

	int32 Id; // Item Class ?? ???? Code / Save/Load ?? ???
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= MemberVariable)
	bool bIsEnabled; // Save/Load ?? ???
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MemberVariable)
	FName Name; // ?¥í???? ???? ??? ???
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MemberVariable)
	int32 Amount; // ????
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MemberVariable)
	//dictionary<string, string> Properties; // ??? ???

private:
	friend FArchive& operator<<(FArchive& Ar, FPickableObjectData& Data)
	{
		Ar << Data.bIsEnabled;
		Ar << Data.Id;
		Ar << Data.Name;
		Ar << Data.Amount;
		return Ar;
	}
};

//UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DELEGATE_OneParam(FOnPickUpVoid, FPickableObjectData);

/*
 * - PickUp ??????? 
 * 1. PickableActor->CanInteract() : true
 * 2. PickableActor->GetItemData() : FPickableObjectData
 * 3. PickableActor->PerformInteract() : void
 *	-> PickableActor->DestroySelf() : void
 */

UCLASS()
class ZOMBIEPROJECT_API APickableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickableActor();

	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual bool CanInteract_Implementation(AActor* Interactor) const;

	virtual void PerformInteract_Implementation(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	void DisplayInteractWidget(AActor* Interactor);

	/*FPickableActorData GetPickableActorData() const {
		FPickableActorData Data{
			InteractTag,
			1,
			GetInteractTag()
		};
		return Data;
	};*/
	

	EInteractionType GetInteractTag_Implementation() const;
	

	//void SaveDataToSaveFile()
	//{
	//	UGameInstance* gInstance = GetWorld()->GetGameInstance();
	//	//gInstance = Cast<ULevelInfoManager>(gInstance);
	//	//gInstance->SaveActorEnabled(Name, this->Data.GetIsEnabled());
	//	//gInstance->SaveActorData(this->Data);
	//}

	//void LoadDataFromSaveFile()
	//{
	//	UGameInstance* gInstance = GetWorld()->GetGameInstance();
	//	//gInstance = Cast<ULevelInfoManager>(gInstance);
	//	//bool _bIsEnabled = gInstance->LoadActorEnabled(Name); // Load Flag First
	//	// 
	//	//if (_bIsEnabled == false)
	//	//{
	//	//	Destroy();
	//	//}
	//	//else
	//	//{
	//	//	Data = gInstance->LoadActorData(Name);
	//	//}
	//};


	virtual void DestroySelf();
	

	friend FArchive& operator<<(FArchive& Ar, const APickableActor& PickableActor)
	{
		auto Data = PickableActor.ItemData;
		Ar << Data;
		// Ar << PickableActor.InteractTag; // some arbitrary data
		return Ar;
	};

public:
	//@region FPickableObjectData Getter/Setter
	// Destroy Self and Return Serializable Data
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	FPickableObjectData GetItemData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = InteractObject)
	void SetItemData(bool bIsEnabled, int32 Id, FName Name, int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	bool GetIsEnabled() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	int32 GetId() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	FName GetItemName() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	int32 GetAmount() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	void SetIsEnabled(bool bNewIsEnabled);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	void SetId(int32 NewId);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	void SetItemName(FName NewName);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = ItemData)
	void SetAmount(int32 NewAmount);
	//@endregion

public:
	FOnPickUpVoid OnPickedUpVoidDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// EventDispatcher when Item is Picked

protected:
	FPickableObjectData ItemData;

private:
	static constexpr EInteractionType InteractTag = EInteractionType::Pickup;
};