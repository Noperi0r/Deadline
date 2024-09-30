#pragma once

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Interact UMETA(DisplayName = "Interact"),
	Pickup UMETA(DisplayName = "Pickup"),
	Open UMETA(DisplayName = "Open"),
	Close UMETA(DisplayName = "Close"),
	Assassinate UMETA(DisplayName = "Assassinate"),
};
