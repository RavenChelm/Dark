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
	//Dont use now
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void StartUpdate();
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void UpdateState(float Progress);
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void StopUpdate();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control")
	void Toggle(const AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Switch Control") // true всегда про состояние "Открыть" 
	void SetState(const AActor* Instigator, bool SwitchState);

};