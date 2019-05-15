// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"
#include "Components/Image.h"


void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UImage>(GetWidgetFromName(TEXT("/Game/TowerofAngra/UI/UI_HP.UI_HP")));
}

void UHUDWidget::UpdateCharacterStat()
{
	HPBar->SetBrushSize(FVector2D(50.f, 50.f));
}

void UHUDWidget::UpdatePlayerState()
{
}
