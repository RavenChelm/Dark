// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCustomDamageType.h"
#include "Components/SkeletalMeshComponent.h"
#include "IWeapon.h"
#include "ElectricGauntletSkeletalMeshComponent.generated.h"

enum class ChargeState : uint8
{
	Discharging,
	Charging,
	FullCharge,
	NullCharge,
};

UCLASS()
class DARK_API UElectricGauntletSkeletalMeshComponent : public USkeletalMeshComponent, public IWeapon
{
	GENERATED_BODY()
	public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	virtual void Attack_Implementation() override;
	virtual void Equip_Implementation(const bool State) override;
	
	virtual void StartCharge_Implementation() override;
	virtual void Charge_Implementation(float DeltaTime) override;
	virtual void EndCharge_Implementation() override;
	void EndFullCharge();

private:
	float CurrentChargeLevel = 0.0f;
	float MaxChargeLevel = 1.0f;
	float DischargeTime = 5.0f;
	float DischargeSpeed = 0.2f; // TODO :: Добавить clamp
	float ChargeSpeed = 0.1f; // TODO :: Добавить clamp

	ChargeState CurrentChargeState = ChargeState::NullCharge;
	FTimerHandle DischargeTimerHandle;

      
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange = 100.f;
    
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRadius = 50.f;
    
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TEnumAsByte<ECollisionChannel> DamageChannel;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FCustomDamageType GauntletDamage;
};
