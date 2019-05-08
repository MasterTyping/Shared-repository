// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "projectlevelGameMode.generated.h"

UCLASS(minimalapi)
class AprojectlevelGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	FName LoadFirstLevel;
	
public:
	AprojectlevelGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};



