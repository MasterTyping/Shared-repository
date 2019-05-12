// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROJECTLEVEL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree *BotBehavior;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		uint8 HP = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
		bool DiedMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion")
		bool HandAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Agro")
		class UBoxComponent *AgroArea;

	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
			UPrimitiveComponent * OtherComponent, int32 OtherbodyIdx, bool bFromSweep, const FHitResult & SweepHit);
	/*UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/



	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;
};
