// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "MainWeapon.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
// Sets default values
AMainWeapon::AMainWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create a Main Weapon

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainWeapon"));
	//Set weapon location Attach to Socket 	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MainWeaponMesh(TEXT("/Game/TowerofAngra/Weapon/side/Side_mesh"));
	//Mesh->SetStaticMesh(MainWeaponMesh.Object->GetStaticMesh());
	RootComponent = Mesh;
	Mesh->SetStaticMesh(MainWeaponMesh.Object);
}

// Called when the game starts or when spawned
void AMainWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

