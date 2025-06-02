// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ILongInteractableSync.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULongInteractableSync : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARK_API ILongInteractableSync
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void StartLongInteractSync(const AActor* Other);
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void TickLongInteractSync(float Progress);
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void CompleteLongInteractSync();
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void CancelLongInteractSync();
};
