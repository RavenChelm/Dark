// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IReactive.h"
#include "ElementalSteamInteractableObjectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UElementalSteamInteractableObjectComponent : public UActorComponent, public IReactive
{
	GENERATED_BODY()

public:
	UElementalSteamInteractableObjectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReactToElement_Implementation(EElementalType ElementType, AActor* Instigator, const FHitResult& Hit) override;
};
