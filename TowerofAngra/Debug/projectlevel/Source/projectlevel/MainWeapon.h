// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainWeapon.generated.h"

UCLASS()
class PROJECTLEVEL_API AMainWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	
};
UENUM()
enum class Weapon : uint8
{
	WEAPON_SWORD	UMETA(DisplayName = "Sword"),
	WEAPON_AXE		UMETA(DisplayName = "Axe"),
	WEAPON_BOW		UMETA(DisplayName = "Bow"),
	WEAPON_HAND		UMETA(DisplayName = "Hand")
};