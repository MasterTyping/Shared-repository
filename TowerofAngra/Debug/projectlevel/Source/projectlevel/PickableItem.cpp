// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "PickableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "projectlevelCharacter.h"
#include "Engine.h"
// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Set RootComponent 
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	//Set item's Mesh
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	//ItemMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ItemMesh->SetupAttachment(RootComponent);
	//Set Collision Volume
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OverlapBegins);

	
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();

	//set Mesh Outline Setting
	ItemMesh->SetRenderCustomDepth(true);
	ItemMesh->CustomDepthStencilValue = 254;


}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, 1, 0));
}

void APickableItem::Show(bool lean)
{
	ECollisionEnabled::Type collision = false ?
		ECollisionEnabled::QueryAndPhysics :
		ECollisionEnabled::NoCollision;

	this->SetActorTickEnabled(false);
	this->ItemMesh->SetVisibility(false);
	this->ItemMesh->SetCollisionEnabled(collision);
}

void APickableItem::OnInteract()
{
	
	AprojectlevelCharacter * Character = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Show(false);
	Character->AddToInventory(this);
	
}

void APickableItem::OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherbodyIdx, bool bFromSweep, const FHitResult & SweepHit)
{
	AprojectlevelCharacter * Character = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr) && ((OtherActor == Character)))
	{
		
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Get Item"));
		OnInteract();

	}
}

