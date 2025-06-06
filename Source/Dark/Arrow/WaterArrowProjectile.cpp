// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterArrowProjectile.h"

#include "Engine/OverlapResult.h"
#include "Dark/InteractObjects/EElementalType.h"
#include "Dark/InteractObjects/Interfaces/IReactive.h"
#include "Particles/ParticleSystemComponent.h"

void AWaterArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	if (WaterSplashParticleComponent)
	{
		WaterSplashParticleComponent->Activate();
	}
	if (OtherActor && OtherActor != this && OtherComp != nullptr)
	{
		bool bReactStatus = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit Water Arrow"));
		if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				if (IReactive::Execute_ReactToElement(Comp, CurrentElement, this, Hit) && !bReactStatus)
				{
					bReactStatus = true;
				}
			}
		}
		if (!bReactStatus)
		{
			if (CanSpawnWaterPuddle(Hit))
			{
				SpawnWaterPuddle(Hit);
			}
		}
	}
}

void AWaterArrowProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor && OtherActor != this && OtherComp != nullptr){

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Overlap Water Arrow"));
		if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				IReactive::Execute_ReactToElement(Comp, CurrentElement, this, SweepResult); 
			}
		}
	}
}

bool AWaterArrowProjectile::CanSpawnWaterPuddle(const FHitResult& Hit) const
{
    const float SurfaceAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector)));
    if (SurfaceAngle > MaxPuddleAngle)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Surface vertical for water puddle"));
        DrawDebugDirectionalArrow(GetWorld(),Hit.ImpactPoint,Hit.ImpactPoint + Hit.ImpactNormal * 50.0f,
        	10.0f,FColor::Red,false,5.0f,0,2.0f);
        return false;
    }
    
    const float PuddleRadius = 100.0f; // TODO :: MAGIC NUMBES
	const FVector BoxExtent = FVector(PuddleRadius, PuddleRadius, 5.0f);
    const FVector PuddleLocation = Hit.ImpactPoint + Hit.ImpactNormal * 6.0f;
    
  	DrawDebugBox(GetWorld(), PuddleLocation, BoxExtent, FColor::Green, false, 5.0f,0,2.0f);

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.bFindInitialOverlaps = true;

	GetWorld()->OverlapMultiByChannel(OverlapResults,PuddleLocation,FQuat::Identity,
		ECC_WorldStatic, FCollisionShape::MakeBox(BoxExtent),QueryParams);
	
	if (OverlapResults.Num() > 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Not enough space for puddle"));
        return false;
    }
    
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Spawn water puddle is possible"));
    return true;
}

void AWaterArrowProjectile::SpawnWaterPuddle(const FHitResult& Hit)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    const FRotator PuddleRotation = Hit.ImpactNormal.Rotation();
    const FVector SpawnLocation = Hit.ImpactPoint;
    
    if (GetWorld())
    {
	    GetWorld()->SpawnActor<AActor>(WaterPuddle, SpawnLocation, FQuat::Identity.Rotator(), SpawnParams);
    }
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Spawn water puddle was"));
}