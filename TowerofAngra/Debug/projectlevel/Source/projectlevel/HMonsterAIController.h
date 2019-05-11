// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "HMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLEVEL_API AHMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHMonsterAIController();

	virtual void Possess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
	
	
};
