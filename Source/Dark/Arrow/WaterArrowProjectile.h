// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkArrowProjectile.h"
#include "WaterArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API AWaterArrowProjectile : public ADarkArrowProjectile
{
	GENERATED_BODY()
public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
