#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UObjectiveInterface : public UInterface
{
	GENERATED_BODY()
};
class ZOMBIEPROJECT_API IObjectiveInterface
{
	GENERATED_BODY()
public:

	void BindOnDeathEvent();
};
