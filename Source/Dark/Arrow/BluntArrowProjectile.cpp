// Fill out your copyright notice in the Description page of Project Settings.


#include "BluntArrowProjectile.h"
#include "../DarkCharacter.h"
#include "Dark/Hands/CrossbowSkeletalMeshComponent.h"
#include "Dark/InteractObjects/EElementalType.h"
#include "Dark/InteractObjects/Interfaces/IReactive.h"
#include "Engine/GameEngine.h"


void ABluntArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	if (OtherActor && OtherActor != this && OtherComp != nullptr){

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Hit Blunt Arrow"));
		if (TArray<UActorComponent*> Components = OtherActor->GetComponentsByInterface(UReactive::StaticClass()); Components.Num() > 0)
		{
			for (UActorComponent* Comp : Components)
			{
				// TODO :: динамически отслеживать тип стрелы из переменной, чтобы иметь возможность менять тип взависимости от того, через что она пролетает
				IReactive::Execute_ReactToElement(Comp, EElementalType::None, this, Hit); 
			}
		}
	}
}

