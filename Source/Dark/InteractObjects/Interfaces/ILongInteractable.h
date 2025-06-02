// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ILongInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULongInteractable : public UInterface
{
	GENERATED_BODY()
};

class DARK_API ILongInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void StartLongInteract(const AActor* Other);
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void TickLongInteract(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void CompleteLongInteract();
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void CancelLongInteract();
};
