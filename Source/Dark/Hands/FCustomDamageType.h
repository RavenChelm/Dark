#pragma once
#include "CoreMinimal.h"
#include "FCustomDamageType.generated.h"
// #include "Dark/Arrow/EDamageType.h"
// #include "Dark/InteractObjects/EElementalType.h"


UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Bludgeoning		UMETA(DisplayName="Bludgeoning"),
	Piercing		UMETA(DisplayName="Piercing"),
	Fire			UMETA(DisplayName="Fire"),
	Electric		UMETA(DisplayName="Electric"),
	Water			UMETA(DisplayName="Water"),
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	None UMETA(DisplayName="None"),
	Fire UMETA(DisplayName="Fire"),
	Water UMETA(DisplayName="Water"),
	Electric UMETA(DisplayName="Electric"),
};

USTRUCT(BlueprintType)
struct DARK_API FCustomDamageType
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EElementalType ElementalType = EElementalType::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDamageType MainDamageType = EDamageType::Bludgeoning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDamageType SecondaryDamageType = EDamageType::Bludgeoning;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MainDamageAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SecondaryDamageAmount = 0;
};