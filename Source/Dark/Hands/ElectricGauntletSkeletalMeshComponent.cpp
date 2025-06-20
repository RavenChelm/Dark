// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricGauntletSkeletalMeshComponent.h"

#include "Dark/DarkCharacter.h"
#include "Dark/InteractObjects/Interfaces/IDamageable.h"
#include "Dark/InteractObjects/Interfaces/IReactive.h"
#include "Kismet/GameplayStatics.h"

void UElectricGauntletSkeletalMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CurrentChargeState == ChargeState::Discharging)
	{
		CurrentChargeLevel -= DeltaTime * DischargeSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
FString::Printf(TEXT("Current Charge Level: %f"), CurrentChargeLevel));
		if (CurrentChargeLevel <= 0)
		{
			CurrentChargeLevel = 0;
			CurrentChargeState = ChargeState::NullCharge;
		}
	}
}

void UElectricGauntletSkeletalMeshComponent::Attack_Implementation()
{
	FCustomDamageType CurrentGauntletDamage = GauntletDamage;
	if (CurrentChargeLevel == 0)
	{
		CurrentGauntletDamage.ElementalType = EElementalType::None;
	}
	else
	{
		CurrentGauntletDamage.MainDamageAmount *= CurrentChargeLevel;
	}
	
	const FVector AttackOrigin = GetComponentLocation();
	TArray<FHitResult> HitResults;
	const FVector EndLocation = AttackOrigin + Cast<ADarkCharacter>(GetOwner())->GetFirstPersonCameraComponent()->GetForwardVector() * AttackRange;

	const bool bDebugEnabled = true; 
	const EDrawDebugTrace::Type DebugDrawType = bDebugEnabled ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	const float DebugDrawDuration = 2.0f; 
	const FLinearColor DebugTraceColor = FLinearColor::Red;
	const FLinearColor DebugHitColor = FLinearColor::Green;
    
	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		AttackOrigin,
		EndLocation,
		AttackRadius,
		UEngineTypes::ConvertToTraceType(DamageChannel),
		false,
		TArray<AActor*>{GetOwner()}, 
		DebugDrawType,
		HitResults,
		true, 
		DebugTraceColor,
		DebugHitColor,
		DebugDrawDuration
	);
	
	for (const FHitResult& Hit : HitResults)
	{
		
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;
		if (bDebugEnabled)
		{
			DrawDebugPoint(
				GetWorld(),
				Hit.ImpactPoint,
				10.0f, 
				FColor::Emerald,
				false,
				DebugDrawDuration
			);
            
			DrawDebugLine(
				GetWorld(),
				AttackOrigin,
				Hit.ImpactPoint,
				FColor::Cyan,
				false,
				DebugDrawDuration,
				0,
				1.0f
			);
		}
		if (TArray<UActorComponent*> Components = HitActor->GetComponentsByInterface(UDamageable::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				IDamageable::Execute_ApplyDamage(Comp, nullptr, GetOwner(), CurrentGauntletDamage); // TODO:: Возможно компонентный подход в этом случае не самый удачный
			}
		}
		if (TArray<UActorComponent*> Components = HitActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				IReactive::Execute_ReactToElement(Comp, CurrentGauntletDamage.ElementalType, GetOwner(), Hit); 
			}
		}
	}
	CurrentChargeLevel = 0;
}

void UElectricGauntletSkeletalMeshComponent::Equip_Implementation(const bool State)
{
	SetupAttachment(Cast<ADarkCharacter>(GetOwner())->GetFirstPersonCameraComponent());
	SetVisibility(State);
	CurrentChargeState = ChargeState::NullCharge;
	CurrentChargeLevel = 0;
}

void UElectricGauntletSkeletalMeshComponent::StartCharge_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start Charge"));
	if (CurrentChargeState != ChargeState::Discharging)
	{
		CurrentChargeState = ChargeState::Charging;
	}
}

void UElectricGauntletSkeletalMeshComponent::Charge_Implementation(float DeltaTime)
{
	if (CurrentChargeState == ChargeState::Charging)
	{
		CurrentChargeLevel += DeltaTime * ChargeSpeed;
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
        FString::Printf(TEXT("Current Charge Level: %f"), CurrentChargeLevel));
        if (CurrentChargeLevel >= MaxChargeLevel)
        {
        	CurrentChargeLevel = 1.0f;
        	EndCharge_Implementation();
        }
	}

}

void UElectricGauntletSkeletalMeshComponent::EndCharge_Implementation()
{
	if (CurrentChargeState == ChargeState::Charging)
	{
		CurrentChargeState = ChargeState::FullCharge;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("End Charge"));
        GetWorld()->GetTimerManager().SetTimer(DischargeTimerHandle, this, &UElectricGauntletSkeletalMeshComponent::EndFullCharge, DischargeTime ,false);
	}
}
void UElectricGauntletSkeletalMeshComponent::EndFullCharge()
{
	CurrentChargeState = ChargeState::Discharging;
}