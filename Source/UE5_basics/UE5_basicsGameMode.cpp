// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5_basicsGameMode.h"
#include "UE5_basicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5_basicsGameMode::AUE5_basicsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
