// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ISwitchControllable.h"
#include "ControlledGateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UControlledGateComponent : public UActorComponent, public ISwitchControllable
{
	GENERATED_BODY()
	bool bItBlock = false;
	float LerpAlpha = 0;
public:
	UControlledGateComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Toggle_Implementation(const AActor* Instigator) override;
	virtual void UpdateState_Implementation(float Progress) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	float Speed = 5;

};
