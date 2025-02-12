// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DarkBowComponent.generated.h"

/**
 *
 */
class ADarkCharacter;


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARK_API UDarkBowComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ADarkArrowProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	float ArrowForceCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ArrowForceMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ArrowForceIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UDarkBowComponent();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeapon(ADarkCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullArrow();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ShootArrow();

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ADarkCharacter* Character;
};
