// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "IWeapon.h"
#include "SwordSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API USwordSkeletalMeshComponent : public USkeletalMeshComponent, public IWeapon
{
	GENERATED_BODY()
	public:
	virtual void Attack_Implementation() override;
};
