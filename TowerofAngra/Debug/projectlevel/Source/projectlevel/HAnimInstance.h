// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "HAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLEVEL_API UHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UHAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
};
