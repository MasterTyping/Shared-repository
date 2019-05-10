// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "projectlevelGameMode.h"
#include "projectlevelCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "projectlevelCharacter.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "UIPlayerController.h"
AprojectlevelGameMode::AprojectlevelGameMode()
{
	LoadFirstLevel = "Stage1";
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerPawnBPClass(TEXT("/Game/TowerofAngra/Character/MagicianCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	

	FLatentActionInfo ActionInfo;
	UGameplayStatics::LoadStreamLevel(this, LoadFirstLevel, true, true, ActionInfo);

	PlayerControllerClass = AUIPlayerController::StaticClass();
}

void AprojectlevelGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
