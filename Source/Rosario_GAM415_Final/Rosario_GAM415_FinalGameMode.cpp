// Copyright Epic Games, Inc. All Rights Reserved.

#include "Rosario_GAM415_FinalGameMode.h"
#include "Rosario_GAM415_FinalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARosario_GAM415_FinalGameMode::ARosario_GAM415_FinalGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
