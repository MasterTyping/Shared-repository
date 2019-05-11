// Fill out your copyright notice in the Description page of Project Settings.

#include "HMonster.h"
#include "HMonsterAIController.h"
#include "HAnimInstance.h"

// Sets default values
AHMonster::AHMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/Golem/Mesh/SK_Golem.SK_Golem"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// Anim
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> GOLEM_ANIM(TEXT("/Game/Golem/Animations/GolemAnimBlueprint.GolemAnimBlueprint_C"));
	if (GOLEM_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GOLEM_ANIM.Class);
	}
	AIControllerClass = AHMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AHMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

