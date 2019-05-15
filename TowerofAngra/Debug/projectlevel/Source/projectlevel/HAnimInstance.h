// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Animation/AnimInstance.h"
#include "HAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnNextAttackHitCheckDelegate);

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

	void PlayAttackMontage();

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnNextAttackHitCheckDelegate OnAttackHitCheck;
private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
