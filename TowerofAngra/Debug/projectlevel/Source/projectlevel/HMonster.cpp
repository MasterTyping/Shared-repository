// Fill out your copyright notice in the Description page of Project Settings.

#include "HMonster.h"
#include "HMonsterAIController.h"
#include "HAnimInstance.h"
#include "Components/WidgetComponent.h"

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

	// UI
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/TowerofAngra/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	AIControllerClass = AHMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IsAttacking = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("HCharater"));
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
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
}

void AHMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto AnimInstance = Cast<UHAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &AHMonster::OnAttackMontageEnded);
}

// Called to bind functionality to input
void AHMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHMonster::Attack()
{
	if (IsAttacking)return;
	auto AnimInstance = Cast<UHAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance)return;

	AnimInstance->PlayAttackMontage();
	IsAttacking = true;
}

void AHMonster::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

