// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "CrossbowSkeletalMeshComponent.generated.h"

class AArrowProjectile;

UENUM(BlueprintType)
enum class EArrowType : uint8
{
	Sharp,
	Fire,
	Water,
	Electric,
	Blunt
};

USTRUCT(BlueprintType)
struct FArrowData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AArrowProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentAmmo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, EArrowType, Type, int32, CurrentAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReloadStartSignature);
UCLASS()
class DARK_API UCrossbowSkeletalMeshComponent : public USkeletalMeshComponent, public IWeapon
{
	GENERATED_BODY()
public:
	UCrossbowSkeletalMeshComponent();
	virtual void Attack_Implementation() override;
	virtual void Equip_Implementation() override;
	UFUNCTION(BlueprintCallable)
	bool AddAmmunition(EArrowType Type, int32 AmmoCount);
	
	UFUNCTION(BlueprintCallable)
	void SwitchArrowType(EArrowType NewType);

		
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChangedSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable)
	FOnReloadStartSignature OnReloadStart;

	
	UPROPERTY(EditDefaultsOnly, Category = "Ammunition")
	TMap<EArrowType, FArrowData> ArrowInventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammunition")
	EArrowType CurrentArrowType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammunition")
	float ReloadTime = 1.5f;

protected:
	virtual void BeginPlay() override;

private:
	bool bIsReloading = false;
	FTimerHandle ReloadTimerHandle;
	
	void FireArrow();
	void StartReload();
	void FinishReload();
	bool CanShoot() const;
};

