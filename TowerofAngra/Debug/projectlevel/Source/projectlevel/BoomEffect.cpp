// Fill out your copyright notice in the Description page of Project Settings.

#include "BoomEffect.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
ABoomEffect::ABoomEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Explosion"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectParticle(TEXT("/Game/StarterContent/Particles/P_Fire"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetTemplate(EffectParticle.Object);
	Effect->SetupAttachment(RootComponent);
	Effect->SetWorldRotation(FRotator(0, 0, 0), false, NULL, ETeleportType::None);
	Effect->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
}

// Called when the game starts or when spawned
void ABoomEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoomEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

