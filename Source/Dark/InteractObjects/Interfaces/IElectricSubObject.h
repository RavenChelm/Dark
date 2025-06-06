// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IElectricSubObject.generated.h"

UINTERFACE()
class UElectricSubObject : public UInterface
{
	GENERATED_BODY()
};


class DARK_API IElectricSubObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void ReactToShortCircuit(const AActor* Other);
	//Это странный кусок кода, но он нужен для того, чтобы вызывать блокировку из ElectrificableObjectComponent у разных подключённых к нему объектов
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void SetBlock(const bool Block);
};