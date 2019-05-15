// Fill out your copyright notice in the Description page of Project Settings.

#include "HAnimInstance.h"


UHAnimInstance::UHAnimInstance()
{
	CurrentPawnSpeed = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Golem/Animations/SK_Golem_Skeleton_Montage.SK_Golem_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void UHAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UHAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UHAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}
