// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrowProjectile.h"
#include "ElectricArrowProjectile.generated.h"

UCLASS()
class DARK_API AElectricArrowProjectile : public AArrowProjectile
{
	GENERATED_BODY()

public:
	AElectricArrowProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
