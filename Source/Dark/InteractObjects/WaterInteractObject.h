// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WaterInteractObject.generated.h"

class UParticleSystemComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARK_API UWaterInteractObject : public UStaticMeshComponent
{
	GENERATED_BODY()
private:
	UParticleSystemComponent* ParticleSystemComponent;
public:
	void BeginPlay();
	void ActStateUp() const;
	void ActStateDown() const;
};
