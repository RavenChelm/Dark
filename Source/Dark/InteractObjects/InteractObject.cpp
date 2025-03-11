// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractObject.h"
#include "Engine/GameEngine.h"

void UInteractObject::Act() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Interact Actor do something"));
}
