// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IReactive.h"
#include "ElectrificableObjectComponent.generated.h"

UENUM(BlueprintType)
enum class EStateElectricObject: uint8
{
	TurnedOff	UMETA(DisplayName="TurnedOff"),
	TurnedOn	UMETA(DisplayName="TurnedOn"),
	Wet			UMETA(DisplayName="Wet"),
	Broken		UMETA(DisplayName="Broken"),
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UElectrificableObjectComponent : public UActorComponent, public IReactive
{
	GENERATED_BODY()
public:
	UElectrificableObjectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReactToElement_Implementation(EElementalType& ElementType, AActor* Instigator, const FHitResult& Hit) override;
	void SwitchTurn(const EStateElectricObject& State);
	void ShortCircuit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<AActor*> ControlledButtons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStateElectricObject CurrentState;
};
