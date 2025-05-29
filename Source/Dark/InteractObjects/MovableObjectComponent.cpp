// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableObjectComponent.h"
#include "../Hands/HandsControllerComponent.h"
#include "../DarkCharacter.h"

UMovableObjectComponent::UMovableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMovableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMovableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Character) GetOwner()->SetActorLocation( Character->GetGrabPoint()->GetComponentLocation(),false, nullptr, ETeleportType::TeleportPhysics);
}

void UMovableObjectComponent::Interact_Implementation(const AActor* Other)
{
	if (!Other) return;

	Character = Cast<ADarkCharacter>(Other);
	if (!Character) return;
	Character->GetHandsController()->SetInteractItem(this);
	UPrimitiveComponent* ObjectMesh = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (!ObjectMesh) return;

	ObjectMesh->SetSimulatePhysics(false);

	ObjectMesh->AttachToComponent(Character->GetGrabPoint(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	ObjectMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); 
}

void UMovableObjectComponent::Throw_Implementation(const FVector& Direction, float Force)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("throwing object"));
	UPrimitiveComponent* ObjectMesh = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (!ObjectMesh) return;
	
	ObjectMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ObjectMesh->SetSimulatePhysics(true);
	ObjectMesh->AddImpulse(Direction * Force, NAME_None, true);
	ObjectMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Character = nullptr;
}

void UMovableObjectComponent::Drop_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Droping object"));
	UPrimitiveComponent* ObjectMesh = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (!ObjectMesh) return;
	
	ObjectMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	ObjectMesh->SetSimulatePhysics(true);
	ObjectMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	Character = nullptr;
}
//Версия с физичным перетаскиванием 
// void UMovableObjectComponent::Interact(const AActor* Other)
// {
// 	if (!Other) return;
//
// 	Character = Cast<ADarkCharacter>(Other);
// 	if (!Character)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Other is not a DarkCharacter!"));
// 		return;
// 	}
//
// 	UPhysicsHandleComponent* PhysicsHandle = Character->GetPhysicsHandleComponent();
// 	if (!PhysicsHandle)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found!"));
// 		return;
// 	}
//
// 	UPrimitiveComponent* ObjectPhysicsComponent = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
// 	if (!ObjectPhysicsComponent)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("No PrimitiveComponent found!"));
// 		return;
// 	}
//
// 	USceneComponent* GrabPoint = Character->GetGrabPoint();
// 	if (!GrabPoint)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("GrabPoint not found!"));
// 		return;
// 	}
//
// 	// Если объект уже захвачен, отпускаем его
// 	if (PhysicsHandle->GrabbedComponent)
// 	{
// 		PhysicsHandle->ReleaseComponent();
// 		return;
// 	}
//
// 	// Захватываем объект
// 	PhysicsHandle->GrabComponentAtLocationWithRotation(
// 		ObjectPhysicsComponent,
// 		NAME_None,
// 		GrabPoint->GetComponentLocation(),
// 		GrabPoint->GetComponentRotation()
// 	);
//
// 	// Настраиваем коллизии
// 	ObjectPhysicsComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
// }
// void UMovableObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	if (Character && Character->GetPhysicsHandleComponent() && Character->GetPhysicsHandleComponent()->GrabbedComponent)
// 	{
// 		Character->GetPhysicsHandleComponent()->SetTargetLocationAndRotation(
// 			Character->GetGrabPoint()->GetComponentLocation(),
// 			Character->GetGrabPoint()->GetComponentRotation()
// 		);
// 	}
// }