// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARK_API IWeapon
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Attack();
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Equip(const bool State);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void StartCharge();
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Charge(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void EndCharge();
};
