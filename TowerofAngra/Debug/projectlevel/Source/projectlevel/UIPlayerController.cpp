// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "UIPlayerController.h"
//#include "projectlevelCharacter.h"
#include "HUDWidget.h"


AUIPlayerController::AUIPlayerController()
{
	// Player UI 수정해야함.
	static ConstructorHelpers::FClassFinder<UHUDWidget> UI_HUD_C(TEXT("/Game/TowerofAngra/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

UHUDWidget * AUIPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
}

void AUIPlayerController::SetupInputComponent()
{
}
