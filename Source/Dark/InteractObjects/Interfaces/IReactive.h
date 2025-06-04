// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IReactive.generated.h"

enum class EElementalType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UReactive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARK_API IReactive
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	bool ReactToElement(EElementalType ElementType, AActor* Instigator, const FHitResult& Hit);
};
