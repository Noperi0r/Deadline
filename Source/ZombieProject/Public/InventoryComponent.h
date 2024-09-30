// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
// TODO: Remove weapon include and make enum header, which applies to all 
#include "Weapon/Weapon.h"
#include "Player/ZombiePlayer.h"
#include "InventoryComponent.generated.h"

//#include "ItemData.h"

// TODO: Change this to EInventoryIndex enum
#define MELEEINDEX 0
#define RANGEINDEX 1
#define THROWINDEX 2

// TODO: Apply these to Num_max variables in player class
#define NUM_MaxRangeSlot 2
#define NUM_MaxThrowingSlot 3

UENUM(BlueprintType)
enum class EInventoryIndex : uint8
{
	INVENTORY_MELEE UMETA(DisplayName = "Inventory Melee"),
	INVENTORY_RANGE UMETA(DisplayName = "Inventory Range"),
	INVENTORY_THROWING UMETA(DisplayName = "Inventory Throwing")
};

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

	// Weapon Array Info

	// Melee
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MeleeNums;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> MeleeCodes;
	
	// Range
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RangeNums;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> RangeCodes;
	
	// Throwing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ThrowingNums;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EThrowingTag, int32> ElementNumThrowings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EThrowingTag, int32> MaxNumThrowings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EThrowingTag, int32> ThrowingCodes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumBullets;

	// Player Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Hp;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Stamina;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EPlayerMode CurrentPlayerMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RecentMeleeIdx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RecentRangeIdx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 RecentThrowingIdx;


	FSaveData()
	{
		for (int i = 1; i < (int)EThrowingTag::LAST; ++i)
		{
			ElementNumThrowings.Emplace((EThrowingTag)i);
			MaxNumThrowings.Emplace((EThrowingTag)i);
		}
	}
};

USTRUCT()
struct FWeapon2DArray
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<class AWeapon*> Arr;

public:
	AWeapon* operator[] (const int32 Idx) const
	{
		return Arr[Idx];
	}

	void Add(AWeapon* Weapon)
	{
		Arr.Add(Weapon);
	}

	void Emplace(AWeapon* Weapon)
	{
		Arr.Emplace(Weapon);
	}

	AWeapon* Pop()
	{
		return Arr.Pop();
	}

	AWeapon* Last() const
	{
		return Arr.Last();
	}

	int32 Num() const
	{
		return Arr.Num();
	}

	FWeapon2DArray()
	{
		Arr = TArray<class AWeapon*>();
	}
};

USTRUCT(BlueprintType, Atomic)
struct FInventoryData 
{
	GENERATED_BODY()

	//FInventoryData() : NumBricks(0), NumBullets(0), MeleeWeaponCode(0), RangeWeaponCode(0), ThrowWeaponCode(0), Weapons(nullptr) {};
	FInventoryData() : NumBullets(0), MeleeWeaponCode(0), RangeWeaponCode(0), ThrowWeaponCode(0)
	{
		Weapon_Melee.Init(nullptr, 1);
		Weapon_Range.Init(nullptr, 2);
		Weapon_Throwing.Init(FWeapon2DArray(), 2);
		
		for (int i = 1; i < (int)EThrowingTag::LAST; ++i)
		{
			CurNumThrowings.Emplace((EThrowingTag)i);
			MaxNumThrowings.Emplace((EThrowingTag)i);
		}

	};

	FInventoryData(const int32 Brick, const int32 Bullet, const int32 MeleeWeaponCode, const int32 RangeWeaponCode, const int32 ThrowWeaponCode)
		:	MeleeWeaponCode(MeleeWeaponCode), 
			RangeWeaponCode(RangeWeaponCode), 
			ThrowWeaponCode(ThrowWeaponCode), 
			NumBullets(Bullet) 
	{
		Weapon_Melee.Init(nullptr, 1);
		Weapon_Range.Init(nullptr, 2);
		Weapon_Throwing.Init(FWeapon2DArray(), 2);

		for (int i = 1; i < (int)EThrowingTag::LAST; ++i)
		{
			CurNumThrowings.Emplace((EThrowingTag)i);
			MaxNumThrowings.Emplace((EThrowingTag)i);
		}
	};

	/*
	* Weapon Code 
	* If it's 0, means that None item is in the inventory
	* Melee weapon code starts with 100, comes to 101, 102, ...
	* Range weapon code starts with 200, comes to 201, 202, ...
	* Throw weapon code starts with 300, comes to 301, 302, ...
	*/
	int32 MeleeWeaponCode;
	int32 RangeWeaponCode;
	int32 ThrowWeaponCode;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<EThrowingTag, int32> CurNumThrowings;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<EThrowingTag, int32> MaxNumThrowings;

	int32 NumBullets;

	//TStaticArray<class AWeapon*,3> Weapons; // 0: Melee, 1: Range, 2: Throw

	//class AWeapon* Weapon_Melee;

	UPROPERTY()
	TArray<class AWeapon*> Weapon_Melee;
	UPROPERTY()
	TArray<class AWeapon*> Weapon_Range;
	//TArray<class AWeapon*> Weapon_Throwing;
	UPROPERTY()
	TArray<FWeapon2DArray> Weapon_Throwing;

private:
	friend FArchive& operator<<(FArchive& Ar, FInventoryData& Data)
	{
		Ar << Data.NumBullets;
		Ar << Data.MeleeWeaponCode;
		Ar << Data.RangeWeaponCode;
		Ar << Data.ThrowWeaponCode;
		return Ar;
	}

};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIEPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// TEST
	FString SerializedData;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FInventoryData Data;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	bool bNoThrowings;

	UPROPERTY(EditAnywhere, Category = "BP Set")
	class UDataTable* WeaponCodeDTable;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetThrowingNum(const int32 Index) const;
	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetBullets() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetMeleeWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetRangeWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetThrowWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "Data")
	AWeapon* Inventory_GetWeapon(int32 WeaponIndex, const int32 WeaponSlot) const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetMeleeBlankSlot();

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 GetRangeBlankSlot();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetThrowingNum(EThrowingTag Type, const int& Value);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetBullets(const int& Value);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetMeleeWeapon(const int& Value);
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetRangeWeapon(const int& Value);
	UFUNCTION(BlueprintCallable, Category = "Data")
	void SetThrowWeapon(const int& Value);

	UFUNCTION(BlueprintCallable, Category = "Data")
	const FString& GetSerializedData();

	UFUNCTION(BlueprintCallable, Category = "Data")
	void Inventory_SetWeapon(const int32 WeaponIndex, class AWeapon* Weapon, int32 WeaponSlot);

	UFUNCTION(BlueprintCallable, Category = "Data")
	void Inventory_ClearWeapon(const int32 WeaponIndex, const int32 WeaponSlot);

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 CheckThrowingNum(EThrowingTag ThrowingType) const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 CheckThrowingTypesNum() const;
	
	UFUNCTION(BlueprintCallable, Category = "Data")
	int32 CheckPrimaryWeaponNum(const int32 WeaponIndex) const;


	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveData(UPARAM(ref) FSaveData& SaveData);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void LoadData(const FSaveData& SaveData);


//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

