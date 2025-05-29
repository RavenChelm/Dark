// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "IInteractable.h"
#include "WaterInteractObject.generated.h"

class UParticleSystemComponent;

/**
 * Переписать на компонент 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, Interfaces="Interactable"))
class DARK_API UWaterInteractObject : public UStaticMeshComponent, public IInteractable
{
	GENERATED_BODY()
	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComponent;
	
public:
	void BeginPlay();
	void ActStateUp() const;
	void ActStateDown() const;
	virtual void Interact_Implementation(const AActor* Other) override;
};
