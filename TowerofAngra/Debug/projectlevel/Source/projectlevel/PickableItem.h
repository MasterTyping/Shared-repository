// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

UENUM(BlueprintType)
enum class EItemTypeEnum : uint8
{
	TYPE_UPGRADE 	UMETA(DisplayName = "upgrade"),
	TYPE_EAT		UMETA(DisplayName = "eat"),
	TYPE_QUEST		UMETA(DisplayName = "quest"),
	TYPE_MONEY		UMETA(DisplayName = "Money")
};
UCLASS()
class PROJECTLEVEL_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Show(bool lean);
	void OnInteract();
	UPROPERTY(EditAnywhere)
		EItemTypeEnum ItemType;
	UPROPERTY(EditAnywhere)
		class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Pickup)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UTexture2D* ItemImage;
	UFUNCTION()
		void OverlapBegins(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
			UPrimitiveComponent * OtherComponent, int32 OtherbodyIdx, bool bFromSweep, const FHitResult & SweepHit);


};
