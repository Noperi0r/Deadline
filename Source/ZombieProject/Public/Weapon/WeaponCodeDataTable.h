// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponCodeDataTable.generated.h"

USTRUCT(BlueprintType)
struct ZOMBIEPROJECT_API FWeaponCodeDataTableRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FName WeaponName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> Weapon;
};
