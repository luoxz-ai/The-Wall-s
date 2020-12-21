// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "The_Wall_2018GameMode.h"
#include "The_Wall_2018PlayerController.h"
#include "The_Wall_2018Character.h"
#include "UObject/ConstructorHelpers.h"

AThe_Wall_2018GameMode::AThe_Wall_2018GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AThe_Wall_2018PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}