// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLEVEL_API AUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AUIPlayerController();
	
	class UHUDWidget* GetHUDWidget() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;
private:
	UPROPERTY()
	class UHUDWidget* HUDWidget;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
