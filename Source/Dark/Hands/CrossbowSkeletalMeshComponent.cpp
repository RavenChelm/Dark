// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossbowSkeletalMeshComponent.h"
#include "../Arrow/ArrowProjectile.h"
#include "../DarkCharacter.h"

UCrossbowSkeletalMeshComponent::UCrossbowSkeletalMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    
	ArrowInventory.Add(EArrowType::Sharp, {nullptr, 15, 15});
	ArrowInventory.Add(EArrowType::Fire, {nullptr, 8, 8});
	ArrowInventory.Add(EArrowType::Water, {nullptr, 5, 5});
	ArrowInventory.Add(EArrowType::Electric, {nullptr, 5, 5});
	ArrowInventory.Add(EArrowType::Blunt, {nullptr, 5, 5});
	CurrentArrowType = EArrowType::Blunt;
}

void UCrossbowSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCrossbowSkeletalMeshComponent::FireArrow()
{
	FArrowData& CurrentArrow = ArrowInventory[CurrentArrowType];
    
	const FVector SpawnLocation = GetSocketLocation("Muzzle");
	const FRotator SpawnRotation = GetSocketRotation("Muzzle");

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = GetOwner()->GetInstigator();

	if (AArrowProjectile* Projectile = GetWorld()->SpawnActor<AArrowProjectile>(
		CurrentArrow.ProjectileClass,
		SpawnLocation,
		SpawnRotation,
		Params))
	{
		const float SpeedMultiplier = 1.2f; 
		Projectile->Launch(Cast<ADarkCharacter>(GetOwner())->GetFirstPersonCameraComponent()->GetForwardVector(), SpeedMultiplier);
	}

	CurrentArrow.CurrentAmmo = FMath::Clamp(CurrentArrow.CurrentAmmo - 1, 0, CurrentArrow.MaxAmmo);
	OnAmmoChanged.Broadcast(CurrentArrowType, CurrentArrow.CurrentAmmo);
}

void UCrossbowSkeletalMeshComponent::StartReload()
{
	if(bIsReloading) return;

	bIsReloading = true;
	OnReloadStart.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(
		ReloadTimerHandle,
		this,
		&UCrossbowSkeletalMeshComponent::FinishReload,
		ReloadTime,
		false
	);
}

void UCrossbowSkeletalMeshComponent::FinishReload()
{
	bIsReloading = false;
}

bool UCrossbowSkeletalMeshComponent::CanShoot() const
{
	return !bIsReloading && 
		   ArrowInventory.Contains(CurrentArrowType) && 
		   ArrowInventory[CurrentArrowType].CurrentAmmo > 0;
}

bool UCrossbowSkeletalMeshComponent::AddAmmunition(EArrowType Type, int32 AmmoCount)
{
	if (ArrowInventory[Type].CurrentAmmo < ArrowInventory[Type].MaxAmmo)
	{
		ArrowInventory[Type].CurrentAmmo++;
		return true;
	}
	return false;
}

void UCrossbowSkeletalMeshComponent::Attack_Implementation()
{
	if(CanShoot())
	{
		FireArrow();
		StartReload();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Reloading"));
	}
}

void UCrossbowSkeletalMeshComponent::Equip_Implementation()
{
	IWeapon::Equip_Implementation();
}

void UCrossbowSkeletalMeshComponent::SwitchArrowType(EArrowType NewType)
{
	if(ArrowInventory.Contains(NewType) && ArrowInventory[NewType].CurrentAmmo > 0)
	{
		CurrentArrowType = NewType;
		OnAmmoChanged.Broadcast(CurrentArrowType, ArrowInventory[CurrentArrowType].CurrentAmmo);
	}
}


