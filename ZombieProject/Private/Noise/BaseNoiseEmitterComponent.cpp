// Fill out your copyright notice in the Description page of Project Settings.


#include "Noise/BaseNoiseEmitterComponent.h"
#include "Noise/EnvironmentManager.h"

#include "Perception/AISense_Hearing.h"

#include <Kismet/KismetSystemLibrary.h>

#include "Math/UnrealMathUtility.h"
#include "Noise/TemporalNoiseEmitterPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBaseNoiseEmitterComponent::UBaseNoiseEmitterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBaseNoiseEmitterComponent::BeginPlay()
{
	Super::BeginPlay();

	InstigatorPawn = Cast<APawn>(GetOwner());

	if (!InstigatorPawn.IsValid())
	{
		InstigatorPawn = Cast<APawn>(GetOwner()->GetInstigator());
	}
}

// Called every frame
//void UBaseNoiseEmitterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

/*
 * Make a Noise Event
 * AI_Hearing으로 감지할 수 있는 Event를 생성한다.
 * MakeNoise 함수는 반드시 NoiseInstigator로 NoiseEmitterComponent를 가진 Pawn을 전달해야한다.
 *
 * @param Loudness Loudness는 소리의 세기로, Tag를 통해 전달되는 FName 값이다.
 * @param NoiseInstigator NoiseInstigator는 소리를 발생시키는 Pawn으로, NoiseInstigator가 nullptr일 경우 Component에 지정된 InstigatorPawn을 사용한다.
 * @param NoiseLocation NoiseLocation은 소리가 발생한 위치이다.
 * @param Range Range는 소리가 전달되는 범위이다.
 */
void UBaseNoiseEmitterComponent::MakeEnvironmentNoise_Implementation(APawn* NoiseInstigator, float Loudness,
                                                                     const FVector& NoiseLocation, float Range)
{
	APawn* TempPawn = nullptr;

	if (NoiseInstigator)
	{
		TempPawn = NoiseInstigator;
	}
	else
	{
		//Spawn a Temporal Noise Emitter Pawn
		auto tempPawn = GetWorld()->SpawnActor<ATemporalNoiseEmitterPawn>(ATemporalNoiseEmitterPawn::StaticClass(), NoiseLocation, FRotator::ZeroRotator);
		tempPawn->SetLifeSpan(1.0f);
		TempPawn = tempPawn;
	}
	check(TempPawn);

	//debug log
	FString b = TempPawn->GetName();

	// UE_LOG(LogTemp, Display, TEXT("Instigator Name : %s"), *b);

	int LoudnessInt = FMath::Floor(Loudness);
	FString LoudnessString = FString::FromInt(LoudnessInt);
	FName LoudnessName = FName(*LoudnessString);

	// 지금 환경에 따라 Hearing Range 를 조정
	// EnvironmentManager를 통해 현재 환경을 가져온다.
	UEnvironmentManager* EnvironmentManager = GetWorld()->GetSubsystem<UEnvironmentManager>();
	float EnvironmentNoiseRangeMultiplier = EnvironmentManager->GetEnvironmentNoiseRangeMultiplier(TempPawn->GetActorLocation());

	// Range 범위 계산
	Range = Range * EnvironmentNoiseRangeMultiplier;

	//Using ReportNoiseEvent to make noise
	UAISense_Hearing::ReportNoiseEvent(this, NoiseLocation, 1, TempPawn, Range, LoudnessName);

	//Spawn Noise Particle 
	if (NoiseParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NoiseParticleSystem, NoiseLocation);
	}
}

/*
 * 주기적으로 소음을 발생시키는 함수
 * ?? 몇초 주기로?
 */
void UBaseNoiseEmitterComponent::MakeContinuousEnvironmentNoise_Implementation(
	APawn* NoiseInstigator, float Loudness, const FVector& NoiseLocation, float Range, float Duration)
{
	// Timer를 통해 주기적으로 소음을 발생시킴
	FTimerHandle noiseTimerHandle;

	FTimerDelegate continuousEvent;
	continuousEvent.BindUFunction(this, FName("MakeEnvironmentNoise_Implementation"), NoiseInstigator, Loudness, NoiseLocation, Range);
	GetWorld()->GetTimerManager().SetTimer(noiseTimerHandle, continuousEvent, 1.0f, true, 0.0f);

	// Duration만큼의 시간이 지나면 소음 발생을 종료함.
	FTimerHandle DurationTimerHandle;
	bool isTimerEnd = false;
	// Timer가 끝나면 TimerHandle을 Clear한다.
	auto durationTimer = [&noiseTimerHandle, this]()
	{
		GetWorld()->GetTimerManager().ClearTimer(noiseTimerHandle);
	};
	GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, durationTimer, Duration, false);

	return;
}
