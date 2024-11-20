// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryComponent.h"
#include "Weapon\Weapon.h"
#include "Weapon\WeaponThrowBase.h"
#include "Weapon\WeaponCodeDataTable.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Data.MaxNumThrowings[EThrowingTag::BRICK] = 5;
	//Data.CurNumThrowings[EThrowingTag::BRICK] = 1;

	Data.MaxNumThrowings[EThrowingTag::GRENADE] = 2;
	//Data.CurNumThrowings[EThrowingTag::GRENADE] = 1;
}

void UInventoryComponent::BeginPlay()
{
	checkf(WeaponCodeDTable, TEXT("InventoryComponent Error: You need to assign WeaponCode data table in inventorycomponent, zombieplayerBP."));

}

int UInventoryComponent::GetThrowingNum(const int32 Index) const { return Data.Weapon_Throwing[Index].Num(); }
int UInventoryComponent::GetBullets() const { return Data.NumBullets; }
int UInventoryComponent::GetMeleeWeapon() const { return Data.MeleeWeaponCode; }
int UInventoryComponent::GetRangeWeapon() const { return Data.RangeWeaponCode; }
int UInventoryComponent::GetThrowWeapon() const { return Data.ThrowWeaponCode; }
void UInventoryComponent::SetThrowingNum(EThrowingTag Type, const int& Value)
{
	bNoThrowings = false;

	if (Data.CurNumThrowings[Type] >= Data.MaxNumThrowings[Type])
		return;
	else if (Value <= Data.CurNumThrowings[Type])
		Data.CurNumThrowings[Type] = Value;

}
void UInventoryComponent::SetBullets(const int& Value) { Data.NumBullets = Value; }
void UInventoryComponent::SetMeleeWeapon(const int& Value) { Data.MeleeWeaponCode = Value; }
void UInventoryComponent::SetRangeWeapon(const int& Value) { Data.RangeWeaponCode = Value; }
void UInventoryComponent::SetThrowWeapon(const int& Value) { Data.ThrowWeaponCode = Value; }

const FString& UInventoryComponent::GetSerializedData() {
	return SerializedData;
}

AWeapon* UInventoryComponent::Inventory_GetWeapon(const int32 WeaponIndex, const int32 WeaponSlot) const 
{

	switch (WeaponIndex)
	{
		case MELEEINDEX:
			if (WeaponSlot > Data.Weapon_Melee.Num() - 1) return nullptr;
			return Data.Weapon_Melee[WeaponSlot];
		case RANGEINDEX:
			if (WeaponSlot > Data.Weapon_Range.Num() - 1) return nullptr;
			return Data.Weapon_Range[WeaponSlot];
		case THROWINDEX:
			if (WeaponSlot > Data.Weapon_Throwing[WeaponSlot].Num() - 1) return nullptr;
			UE_LOG(LogTemp, Warning, TEXT("WeaponName: %s"), *(Data.Weapon_Throwing[WeaponSlot].Last()->GetActorNameOrLabel()));
			return Data.Weapon_Throwing[WeaponSlot][0];
	}
	UE_LOG(LogTemp, Error, TEXT("Inventory_GetWeapon ERROR: Invalid Weapon WeaponIndex"));
	return nullptr;
}

void UInventoryComponent::Inventory_SetWeapon(const int32 WeaponIndex, class AWeapon* Weapon, int32 WeaponSlot)
{
	switch (WeaponIndex)
	{
	case MELEEINDEX:
		Data.Weapon_Melee[WeaponSlot] = Weapon;
		break;

	case RANGEINDEX:
		Data.Weapon_Range[WeaponSlot] = Weapon;
		break;

	case THROWINDEX:
		Data.Weapon_Throwing[WeaponSlot].Emplace(Weapon);
		UE_LOG(LogTemp, Warning, TEXT("%d Slot  Weapon Throwing Size: %d"), WeaponSlot ,Data.Weapon_Throwing[WeaponSlot].Num())

		// FIX 
		//EThrowingTag ThrowingTag = (EThrowingTag)(WeaponSlot + 1);

		//int32 Num = GetThrowingNum(ThrowingTag) + 1;
		//UE_LOG(LogTemp, Warning, TEXT("ThrowingNum: %d"), Num);

		//SetThrowingNum(ThrowingTag, GetThrowingNum(ThrowingTag) + 1);

		break;
	}
	//Data.Weapons[WeaponSlot] = Weapon; 
}

void UInventoryComponent::Inventory_ClearWeapon(const int32 WeaponIndex, const int32 WeaponSlot)
{
	switch (WeaponIndex)
	{
	case MELEEINDEX:
		Data.Weapon_Melee[WeaponSlot] = nullptr;
		break;
	case RANGEINDEX:
		Data.Weapon_Range[WeaponSlot] = nullptr;
		break;
	case THROWINDEX:
		//Data.Weapon_Throwing[WeaponSlot].Pop();
		Data.Weapon_Throwing[WeaponSlot].RemoveAt(0);
		break;
	}
}

int32 UInventoryComponent::CheckThrowingNum(EThrowingTag ThrowingType) const
{
	return Data.Weapon_Throwing[(int8)ThrowingType-1].Num();
}

int32 UInventoryComponent::CheckThrowingTypesNum() const
{
	return Data.Weapon_Throwing.Num();
}

int32 UInventoryComponent::CheckPrimaryWeaponNum(const int32 WeaponIndex) const
{
	switch (WeaponIndex)
	{
		case MELEEINDEX:
			return Data.Weapon_Melee.Num();
			break;
		case RANGEINDEX:
			return Data.Weapon_Range.Num();
			break;
		case THROWINDEX:
			UE_LOG(LogTemp, Warning, TEXT("CheckPrimaryWeaponNum: Use CheckThrowingNum method for throwing weapons"));
			break;
	}
	return -1;
}

int32 UInventoryComponent::GetMeleeBlankSlot()
{
	for (int i = 0; i < Data.Weapon_Melee.Num(); ++i)
	{
		if (!Data.Weapon_Melee[i]) return i;
	}
	return -1;
}

int32 UInventoryComponent::GetRangeBlankSlot()
{
	for (int i = 0; i < Data.Weapon_Range.Num(); ++i)
	{
		if (!Data.Weapon_Range[i]) return i;
	}
	return -1;
}

void UInventoryComponent::SaveData(UPARAM(ref) FSaveData& SaveData)
{
	AZombiePlayer* Player = Cast<AZombiePlayer>(GetOwner());
	checkf(Player, TEXT("InventoryComponent->Savedata: Player Not found error"));

	// Melee
	SaveData.MeleeNums = Data.Weapon_Melee.Num();
	for (int i = 0; i < SaveData.MeleeNums; ++i)
	{
		if(Data.Weapon_Melee[i])
			SaveData.MeleeCodes.Add(Data.Weapon_Melee[i]->ItemCode);
	}
	// Range
	SaveData.RangeNums = Data.Weapon_Range.Num();
	for (int i = 0; i < SaveData.RangeNums; ++i)
	{
		if(Data.Weapon_Range[i])
			SaveData.RangeCodes.Add(Data.Weapon_Range[i]->ItemCode);
	}
	SaveData.NumBullets = Data.NumBullets;

	// Throwing
	SaveData.ThrowingCodes.Emplace(EThrowingTag::BRICK, 300);
	SaveData.ThrowingCodes.Emplace(EThrowingTag::GRENADE, 310);

	SaveData.ThrowingNums = Data.Weapon_Throwing.Num();
	for (int i = 0; i < SaveData.ThrowingNums; ++i) 
	{
		SaveData.ElementNumThrowings[(EThrowingTag)(i + 1)] = Data.Weapon_Throwing[i].Num();
		SaveData.MaxNumThrowings[(EThrowingTag)(i + 1)] = Data.MaxNumThrowings[(EThrowingTag)(i + 1)];
	}

	// Player Info
	SaveData.Hp = Player->Hp;
	SaveData.Stamina = Player->Stamina;
	SaveData.CurrentPlayerMode = Player->CurrentPlayerMode;

	SaveData.RecentMeleeIdx = Player->RecentMeleeWeaponIndex;
	SaveData.RecentRangeIdx = Player->RecentRangeWeaponIndex;
	SaveData.RecentThrowingIdx = Player->RecentThrowingWeaponIndex;

}

void UInventoryComponent::LoadData(const FSaveData& SaveData)
{
	if(!Data.Weapon_Melee.IsEmpty())
		Data.Weapon_Melee.Empty();

	if (!Data.Weapon_Range.IsEmpty())
		Data.Weapon_Range.Empty();

	if (!Data.Weapon_Throwing.IsEmpty())
		Data.Weapon_Throwing.Empty();

	Data.Weapon_Melee.Init(nullptr, SaveData.MeleeNums);
	Data.Weapon_Range.Init(nullptr, SaveData.RangeNums);
	Data.Weapon_Throwing.Init(FWeapon2DArray(), SaveData.ThrowingNums);

	FWeaponCodeDataTableRow* Row = nullptr;
	AZombiePlayer* Player = Cast<AZombiePlayer>(GetOwner());

	// Melee
	for (int i = 0; i < SaveData.MeleeNums; ++i)
	{
		if(SaveData.MeleeCodes.IsEmpty())
			return;
		
		FString Num = FString::FromInt(SaveData.MeleeCodes[i]);
		Row = WeaponCodeDTable->FindRow<FWeaponCodeDataTableRow>(FName(Num), FString("Item none found"));
		
		checkf(Row, TEXT("Melee Itemcode not found in Data table."));
		//if (!Row)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Itemcode matching not found. LoadData Aborted"));
		//	return;
		//}

		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(Row->Weapon);
		Player->LootWeapon(SpawnedWeapon);
	}

	// Range
	for (int i = 0; i < SaveData.RangeNums; ++i)
	{
		if(SaveData.RangeCodes.IsEmpty())
			return;
		
		FString Num = FString::FromInt(SaveData.RangeCodes[i]);
		Row = WeaponCodeDTable->FindRow<FWeaponCodeDataTableRow>(FName(Num), FString("Item none found"));
		
		checkf(Row, TEXT("Range Itemcode not found in Data table."));
		//if (!Row)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Itemcode matching not found. LoadData Aborted"));
		//	return;
		//}

		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(Row->Weapon);
		Player->LootWeapon(SpawnedWeapon);
	}
	Data.NumBullets = SaveData.NumBullets;

	// Throwing
	for (int i = 0; i < SaveData.ThrowingNums; ++i)
	{
		Data.MaxNumThrowings[(EThrowingTag)(i + 1)] = SaveData.MaxNumThrowings[(EThrowingTag)(i + 1)];

		UE_LOG(LogTemp, Warning, TEXT("CurNumThrowing Throwingtag: %d, number: %d"), i+1, Data.CurNumThrowings[(EThrowingTag)(i + 1)]);

		if (Data.CurNumThrowings[(EThrowingTag)(i + 1)] != 0)
		{
			bNoThrowings = false;
		}

		FString Num = FString::FromInt(SaveData.ThrowingCodes[(EThrowingTag)(i + 1)]);
		Row = WeaponCodeDTable->FindRow<FWeaponCodeDataTableRow>(FName(Num), FString("Item none found"));
		
		checkf(Row, TEXT("Range Itemcode not found in Data table."));
		//if (!Row)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Itemcode matching not found. LoadData Aborted"));
		//	return;
		//}

		for (int j = 0; j < SaveData.ElementNumThrowings[(EThrowingTag)(i + 1)]; ++j) 
		{
			AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(Row->Weapon);
			Player->LootWeapon(SpawnedWeapon);
		}
	}

	// Player Properties
	// Player->Hp = SaveData.Hp;
	// Player->Stamina = SaveData.Stamina;
	Player->CurrentPlayerMode = SaveData.CurrentPlayerMode;

	Player->RecentMeleeWeaponIndex = SaveData.RecentMeleeIdx;
	Player->RecentRangeWeaponIndex = SaveData.RecentRangeIdx;
	Player->RecentThrowingWeaponIndex = SaveData.RecentThrowingIdx;

	switch (Player->CurrentPlayerMode)
	{
		case EPlayerMode::MeleeMode:
			Player->WeaponChange(MELEEINDEX, Player->RecentMeleeWeaponIndex);
			break;
		case EPlayerMode::ThrowMode:
			Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
			break;
		case EPlayerMode::RangeMode:
			Player->WeaponChange(RANGEINDEX, Player->RecentRangeWeaponIndex);
			break;
	}

	// switch (Player->CurrentPlayerMode)
	// {
	// 	case EPlayerMode::MeleeMode:
	// 		Player->WeaponChange(MELEEINDEX, Player->RecentMeleeWeaponIndex);
	// 		break;
	// 	case EPlayerMode::RangeMode:
	// 		Player->WeaponChange(RANGEINDEX, Player->RecentRangeWeaponIndex);
	// 		break;
	// 	case EPlayerMode::ThrowMode:
	// 		Player->WeaponChange(THROWINDEX, Player->RecentThrowingWeaponIndex);
	// 		break;
	// 	default:
	// 		checkNoEntry();
	// }
}
