// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "EnemyCharacter.h"
#include "projectlevelCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "SkillEffect.h"
#include "Components/WidgetComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DiedMotion = false;
	HandAttack = false;
	AgroArea = CreateDefaultSubobject<UBoxComponent>(TEXT("AgroArea"));
	AgroArea->SetupAttachment(GetMesh());
	AgroArea->InitBoxExtent(FVector(200, 200, 200));
	AgroArea->SetRelativeLocation(FVector(0, 0, 120));
	//AgroArea->SetGenerateOverlapEvents(true);
	AgroArea->OnComponentBeginOverlap.AddDynamic(this,&AEnemyCharacter::OverlapBegins);
	/*AgroArea->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);*/

	// UI
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/TowerofAngra/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HandAttack = false;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*AprojectlevelCharacter* MainChar = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MainChar->IsOverlappingActor(this))
	{
		HP -= 1;
	}
	}*/
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherbodyIdx, bool bFromSweep, const FHitResult & SweepHit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		AprojectlevelCharacter* MyChar = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		/*TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkillEffect::StaticClass(), FoundActors);*/
		/*if (MyChar)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("On Agro"));
			HandAttack = true;
		}*/
		//for (int i = 0; i < FoundActors.Num(); i++)
		//{
		//	if (FoundActors[i])
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Attacked"));
		//		HP = 0;
		//		FoundActors[i]->Destroy();
		//		//AgroArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//	}
		//}
		
	}
}

//void AEnemyCharacter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
//	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
//{
//	// Other Actor is the actor that triggered the event. Check that is not ourself.  
//	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
//	{
//		ACharacter* MyChar = UGameplayStatics::GetPlayerCharacter(this, 0);
//		if (MyChar)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Off Agro"));
//			HandAttack = false;
//		}
//		else
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("off Agro"));
//		}
//		
//	}
//}

