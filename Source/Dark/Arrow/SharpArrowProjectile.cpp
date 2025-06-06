// Fill out your copyright notice in the Description page of Project Settings.


#include "SharpArrowProjectile.h"

#include "Dark/InteractObjects/EElementalType.h"
#include "Dark/InteractObjects/Interfaces/IReactive.h"


ASharpArrowProjectile::ASharpArrowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASharpArrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASharpArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASharpArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	if (OtherActor && OtherActor != this && OtherComp != nullptr){

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit Sharp Arrow"));
		if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				IReactive::Execute_ReactToElement(Comp, CurrentElement, this, Hit); 
			}
		}
	}
}

void ASharpArrowProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor && OtherActor != this && OtherComp != nullptr){

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Overlap Sharp Arrow"));
		if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				IReactive::Execute_ReactToElement(Comp, CurrentElement, this, SweepResult); 
			}
		}
	}
}

