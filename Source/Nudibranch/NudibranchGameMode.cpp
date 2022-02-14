// Copyright Epic Games, Inc. All Rights Reserved.

#include "NudibranchGameMode.h"
#include "NudibranchHUD.h"
#include "NudibranchCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANudibranchGameMode::ANudibranchGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANudibranchHUD::StaticClass();
}
