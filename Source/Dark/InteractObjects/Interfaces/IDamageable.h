// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

struct FCustomDamageType;

UINTERFACE()
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class DARK_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Damage")
	void ApplyDamage(const AActor* Instigator, const AActor* DamageCauser, const FCustomDamageType& DamageType);
};
