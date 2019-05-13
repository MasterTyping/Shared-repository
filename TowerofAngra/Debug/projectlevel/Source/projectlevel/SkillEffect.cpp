// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "SkillEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ConstructorHelpers.h"
#include "BoomEffect.h"
#include "Engine/World.h"
#include "projectlevelCharacter.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
// Sets default values
ASkillEffect::ASkillEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Setup collision volume & Attach Root Component
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(25.0f);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASkillEffect::OverlapBegins);
	//Collision->collision
	RootComponent = Collision;

	//Skill Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectParticle(TEXT("/Game/StarterContent/Particles/P_Fire"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	Effect->SetTemplate(EffectParticle.Object);
	Effect->SetupAttachment(Collision);
	Effect->SetWorldRotation(FRotator(0, 0, 0), false, NULL, ETeleportType::None);
	Effect->SetWorldScale3D(FVector(0.3, 0.3, 0.3));
	//Setup Projectile Setting
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(Collision);
	ProjectileMovementComponent->InitialSpeed = 300.0f;
	ProjectileMovementComponent->MaxSpeed = 300.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	
	//ProjectileMovementComponent = RootComponent;
}

// Called when the game starts or when spawned
void ASkillEffect::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DTimerhandle, this, &ASkillEffect::EndLifeTime, 3.0f, false, 5.f);
	
}

void ASkillEffect::FireInDirection(const FVector & ShootDirection)
{
	ProjectileMovementComponent->Velocity = (ShootDirection) * ProjectileMovementComponent->InitialSpeed;
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, ProjectileMovementComponent->Velocity.ToString());
}

void ASkillEffect::EndLifeTime()
{
	Destroy();
}

// Called every frame
void ASkillEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, ProjectileMovementComponent->Velocity.ToString());
}

void ASkillEffect::OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherbodyIdx, bool bFromSweep, const FHitResult & SweepHit)
{
	AprojectlevelCharacter *Char = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AEnemyCharacter *Enemy = Cast<AEnemyCharacter>(OtherActor);
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr) && (OtherActor != Char))
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, GetActorLocation().ToString());
		//Destroy();
		/*UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		ABoomEffect* Projectile = World->SpawnActor<ABoomEffect>(ABoomEffect::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);*/
	}
	if(Enemy)
	{
		Enemy->HP = 0;
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, GetActorLocation().ToString());
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		
		
		
		ABoomEffect* Projectile = World->SpawnActor<ABoomEffect>(ABoomEffect::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
		Destroy();
	}
}

