// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "MainPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "projectlevelCharacter.h"
#include <string>
#include "TimerManager.h"
#include <vector>
#include <algorithm>



AMainPlayerController::AMainPlayerController()
{
	SessionId = FMath::RandRange(0, 100);

	// 서버와 연결
	Socket = server::GetSingleton();
	Socket->Init();
	bIsConnected = Socket->Connect("127.0.0.1", 9000);
	UE_LOG(LogClass, Log, TEXT("MainPlayerController!"));
	bNewPlayerEntered = false;
	nPlayers = -1;
	PrimaryActorTick.bCanEverTick = true;
}

AMainPlayerController::~AMainPlayerController()
{
}

int AMainPlayerController::GetSessionId()
{
	return SessionId;
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Controller"));
	Socket->Connect("127.0.0.1", 9000);

}

void AMainPlayerController::BeginPlay()
{
	// 캐릭터 등록
	auto Player = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
	{
		return;
	}
}

void AMainPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

AActor * AMainPlayerController::FindActorBySessionId(TArray<AActor*> ActorArray, const int & SessionId)
{
	for (const auto& Actor : ActorArray)
	{
		AprojectlevelCharacter * swc = Cast<AprojectlevelCharacter>(Actor);
		if (swc && swc->SessionId == SessionId)
			return Actor;
	}
	return nullptr;
}

void AMainPlayerController::RecvWorldInfo(cPlayerInfo * ci)
{
}

void AMainPlayerController::RecvNewPlayer(cPlayer * NewPlayer)
{
}

void AMainPlayerController::SendPlayerInfo()
{
}

bool AMainPlayerController::UpdateWorldInfo()
{
	return false;
}

void AMainPlayerController::UpdatePlayerInfo(const cPlayer & info)
{
}

void AMainPlayerController::UpdateNewPlayer()
{
}
