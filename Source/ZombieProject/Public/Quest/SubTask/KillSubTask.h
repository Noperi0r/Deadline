#pragma once

#include "CoreMinimal.h"
#include "SubTaskBase.h"
#include "KillSubTask.generated.h"

UCLASS(Blueprintable)
class UKillSubTask : public USubTaskBase
{
	GENERATED_BODY()

public:
	UKillSubTask();

	virtual ~UKillSubTask() override;
	
	virtual void BeginPlay();

	virtual bool CanStartTask_Implementation() override;
	
	virtual bool CanCompleteTask_Implementation() override;
};

