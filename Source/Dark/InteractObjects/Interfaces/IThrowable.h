// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IThrowable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UThrowable : public UInterface
{
	GENERATED_BODY()
};

class DARK_API IThrowable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Throw")
	void Throw(const FVector& Direction, float Force);
	UFUNCTION(BlueprintNativeEvent, Category = "Throw")
	void Drop();
};
