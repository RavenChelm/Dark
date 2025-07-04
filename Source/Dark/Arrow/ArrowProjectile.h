// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dark/Hands/FCustomDamageType.h"
#include "GameFramework/Actor.h"
#include "ArrowProjectile.generated.h"

enum class EElementalType : uint8;
enum class EDamageType : uint8;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class DARK_API AArrowProjectile : public AActor // TODO :: Наследование в типах стрел не требуется, разницу в логике можно реализовать с помощью настроек
{
	GENERATED_BODY()
    
public:
	AArrowProjectile();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Launch(const FVector& Direction, float SpeedMultiplier = 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArrowSettings")
	FCustomDamageType ArrowType;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bWasLaunched = false;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactEffect;
};

