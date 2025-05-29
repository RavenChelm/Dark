#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISwitchControllable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USwitchControllable : public UInterface
{
	GENERATED_BODY()
};

class ISwitchControllable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void UpdateState(float Progress);
    
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void Toggle(AActor* Instigator);
};