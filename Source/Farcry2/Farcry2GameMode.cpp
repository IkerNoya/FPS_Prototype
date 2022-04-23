// Copyright Epic Games, Inc. All Rights Reserved.

#include "Farcry2GameMode.h"
#include "Farcry2Character.h"
#include "UObject/ConstructorHelpers.h"

AFarcry2GameMode::AFarcry2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
