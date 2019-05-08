// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameManager.h"
#include "Engine/World.h"
#include "Engine.h"
#include "projectlevelCharacter.h"
#include"server.h"
#include<vector>
#include<string>
#include<algorithm>
#include"Kismet/GameplayStatics.h"


//#define MAX_USER   100
//
//server*   Socket;
//bool is_Connected;
//int RandomID;

// Sets default values
AGameManager::AGameManager()
{
	//// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//RandomID = FMath::RandRange(0, 100);

	//Socket = server::GetSingleton();
	//Socket->InitSock();
	//is_Connected = Socket->Connect("127.0.0.1", 9000);
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//   GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("Gamemanager CLASS TICK()."));

	//if (!is_Connected)
	//{
	//	return;
	//}

	//auto p = Cast<AprojectlevelCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	////
	////
	////
	////   //일단 플레이어 위치 가져오기
	//auto MyLocation = p->GetActorLocation();
	//auto MyRotation = p->GetActorRotation();
	////
	////   
	//Player player;
	//player.ClientID = RandomID;
	//player.X = MyLocation.X;
	//player.Y = MyLocation.Y;
	//player.Z = MyLocation.Z;
	//player.Yaw = MyRotation.Yaw;
	//player.Pitch = MyRotation.Pitch;
	//player.Roll = MyRotation.Roll;
	////
	////   // 플레이어의 아이디와 위치 서버한테 보냄
	//PlayerInfo* pi = Socket->SyncCharacters(player);
	//UWorld* const GameWorld = GetWorld();
	////
	////   //월드 내 액터 수집
	//TArray<AActor*> SpawnedCharacters;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AprojectlevelCharacter::StaticClass(), SpawnedCharacters);

	//for (int i = 0; i < MAX_USER; ++i)
	//{
	//	int PlayerSessionID = pi->WorldCharacterInfo[i].ClientID;

	//	// 유효한 세션 아이디면서 플레이어의 세션아이디가 아닐때
	//	if (PlayerSessionID != -1 && PlayerSessionID != RandomID)
	//	{
	//		// 월드내 해당 세션 아이디와 매칭되는 Actor 검색         
	//		auto Actor = FindActorBySessionId(SpawnedCharacters, PlayerSessionID);
	//		// 해당되는 세션 아이디가 없을 시 월드에 스폰
	//		if (Actor == nullptr)
	//		{
	//			FVector SpawnLocation;
	//			SpawnLocation.X = pi->WorldCharacterInfo[i].X;
	//			SpawnLocation.Y = pi->WorldCharacterInfo[i].Y;
	//			SpawnLocation.Z = pi->WorldCharacterInfo[i].Z;

	//			FRotator SpawnRotation;
	//			SpawnRotation.Yaw = pi->WorldCharacterInfo[i].Yaw;
	//			SpawnRotation.Pitch = pi->WorldCharacterInfo[i].Pitch;
	//			SpawnRotation.Roll = pi->WorldCharacterInfo[i].Roll;

	//			FActorSpawnParameters SpawnParams;
	//			SpawnParams.Owner = this;
	//			SpawnParams.Instigator = Instigator;
	//			SpawnParams.Name = FName(*FString(to_string(pi->WorldCharacterInfo[i].ClientID).c_str()));

	//			ACharacter* const SpawnCharacter = GameWorld->SpawnActor<ACharacter>(WhoToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
	//		}
	//		// 해당되는 세션 아이디가 있으면 위치 동기화
	//		else
	//		{
	//			FVector CharacterLocation;
	//			CharacterLocation.X = pi->WorldCharacterInfo[PlayerSessionID].X;
	//			CharacterLocation.Y = pi->WorldCharacterInfo[PlayerSessionID].Y;
	//			CharacterLocation.Z = pi->WorldCharacterInfo[PlayerSessionID].Z;

	//			FRotator CharacterRotation;
	//			CharacterRotation.Yaw = pi->WorldCharacterInfo[PlayerSessionID].Yaw;
	//			CharacterRotation.Pitch = pi->WorldCharacterInfo[PlayerSessionID].Pitch;
	//			CharacterRotation.Roll = pi->WorldCharacterInfo[PlayerSessionID].Roll;

	//			Actor->SetActorLocation(CharacterLocation);
	//			Actor->SetActorRotation(CharacterRotation);
	//		}
	//	}
	//}
}


void AGameManager::SpawnMonster()
{

	UWorld* world = GetWorld();
	////Spawn Actor with Static Class
	//FActorSpawnParameters Sparam;
	FActorSpawnParameters Sparam;
	Sparam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Sparam.Owner = this;
	Monster = world->SpawnActor<AActor>(AActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), Sparam);
}

AActor * AGameManager::FindActorBySessionId(TArray<AActor*> ActorArray, const int & SessionId)
{
	///*for (const auto& Actor : ActorArray)
	//{
	//	AprojectlevelCharacter * swc = Cast<AprojectlevelCharacter>(Actor);
	//	if (swc && swc->SessionId == SessionId)
	//		return Actor;
	//}//
	return nullptr;
}

