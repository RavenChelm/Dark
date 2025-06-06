// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IElectricSubObject.h"
#include "Interfaces/IInteractable.h"
#include "Interfaces/IReactive.h"
#include "InteractableButtonComponent.generated.h"


class UTimelineComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UInteractableButtonComponent : public UActorComponent, public IInteractable, public IReactive, public IElectricSubObject
{
	GENERATED_BODY()

public:
	UInteractableButtonComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Interact_Implementation(const AActor* Other) override;
	
	virtual bool ReactToElement_Implementation(EElementalType& ElementType, AActor* Instigator, const FHitResult& Hit) override;
	
	virtual void ReactToShortCircuit_Implementation(const AActor* Other) override;
	virtual void SetBlock_Implementation(const bool Block) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<AActor*> ControlledActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bBlockAfterInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	bool bBlock = false;
};
