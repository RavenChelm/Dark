// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IDamageable.h"
#include "DamageableObjectComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARK_API UDamageableObjectComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

	
public:
	UDamageableObjectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ApplyDamage_Implementation(const AActor* Instigator, const AActor* DamageCauser, const FCustomDamageType& DamageType) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArrowSettings")
	int32 Health = 1;
};
