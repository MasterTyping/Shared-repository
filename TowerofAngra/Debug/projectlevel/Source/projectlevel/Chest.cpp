// Fill out your copyright notice in the Description page of Project Settings.

#include "Chest.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChestBody(TEXT("/Game/MultistoryDungeons/Meshes/Props/Treasure_Chest_Base_02"));
	
	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("body"));
	body->SetStaticMesh(ChestBody.Object);
	body->SetupAttachment(RootComponent);
	body->SetRelativeLocation(FVector(0, 0, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChestHead(TEXT("/Game/MultistoryDungeons/Meshes/Props/Treasure_Chest_lid_01"));

	head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("head"));
	head->SetStaticMesh(ChestHead.Object);
	head->SetupAttachment(RootComponent);
	head->SetRelativeLocation(FVector(0, -34, 40));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetRelativeLocation(FVector(0, 0, 100));
	CollisionBox->InitBoxExtent(FVector(100, 100, 100));


}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

