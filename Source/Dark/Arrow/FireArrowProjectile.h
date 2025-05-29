// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArrowProjectile.h"
#include "FireArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API AFireArrowProjectile : public AArrowProjectile
{
	GENERATED_BODY()
public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
