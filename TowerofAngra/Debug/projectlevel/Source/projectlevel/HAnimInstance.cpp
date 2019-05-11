// Fill out your copyright notice in the Description page of Project Settings.

#include "HAnimInstance.h"


UHAnimInstance::UHAnimInstance()
{
	CurrentPawnSpeed = 0.f;
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
