// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkArrowProjectile.h"
#include "FireArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class DARK_API AFireArrowProjectile : public ADarkArrowProjectile
{
	GENERATED_BODY()
public:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
