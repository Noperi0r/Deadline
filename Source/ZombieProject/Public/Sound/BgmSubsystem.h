// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BgmSubsystem.generated.h"

/**
 * BGM을 실행하기 위한 서브 시스템, 레벨에 상관없이 BGM을 재생할 수 있습니다.
 * 레벨을 이동할 때마다 현재 재생 중인 음악은 정지됩니다.
 */
UCLASS()
class ZOMBIEPROJECT_API UBgmSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UBgmSubsystem();
	
	// USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem Interface

	UFUNCTION(BlueprintCallable, Category = "BGM")
	void PlayBgm(int BgmId);
	

private:
	// 변수들
	
	UDataTable* BgmDataTable;

	TSoftObjectPtr<USoundWave> CurrentBgm;

	void OnBgmLoaded(const int BgmId) const;
};
