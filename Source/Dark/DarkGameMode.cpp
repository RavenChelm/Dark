// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkGameMode.h"
#include "DarkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADarkGameMode::ADarkGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
