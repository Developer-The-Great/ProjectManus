// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectManusGameMode.h"
#include "ProjectManusPawn.h"

AProjectManusGameMode::AProjectManusGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AProjectManusPawn::StaticClass();
}
