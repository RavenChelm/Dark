#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadialMenuBase.generated.h"

UCLASS(Abstract, Blueprintable)
class DARK_API URadialMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Radial Menu")
	void UpdateSelection(const int SectorNumber);

	UFUNCTION(BlueprintNativeEvent, Category = "Radial Menu")
	void ActivateSelectedSection(const AActor* Actor);
};