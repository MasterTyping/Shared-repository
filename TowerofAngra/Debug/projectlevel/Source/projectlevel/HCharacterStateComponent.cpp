// Fill out your copyright notice in the Description page of Project Settings.

#include "HCharacterStateComponent.h"


// Sets default values for this component's properties
UHCharacterStateComponent::UHCharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}


// Called when the game starts
void UHCharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHCharacterStateComponent::InitializeComponent()
{
	Super::InitializeComponent();

}


// Called every frame
void UHCharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHCharacterStateComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.f, 100));
}

void UHCharacterStateComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}

float UHCharacterStateComponent::GetAttack() const
{
	return 10.0f;
}

float UHCharacterStateComponent::GetHPRatio() const
{
	return (100.f < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / 100.f);
}

