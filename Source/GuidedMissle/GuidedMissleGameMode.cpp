// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuidedMissleGameMode.h"
#include "GuidedMissleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGuidedMissleGameMode::AGuidedMissleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
