// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "server.h"
#include "MainPlayerController.generated.h"

#define MAX_USER		100

/**
 * 
 */
UCLASS()
class PROJECTLEVEL_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();
	~AMainPlayerController();

	// 세션 아이디 반화
	UFUNCTION(BlueprintPure, Category = "Properties")
		int GetSessionId();

	// 스폰시킬 다른 캐릭터
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACharacter> WhoToSpawn;
	//
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 세션아이디에 매칭되는 액터 반환
	AActor* FindActorBySessionId(TArray<AActor*> ActorArray, const int& SessionId);

	// 소켓으로부터 월드 정보 수신
	void RecvWorldInfo(cPlayerInfo * ci);

	// 새 플레이어 업데이트
	void RecvNewPlayer(cPlayer * NewPlayer);
	//
	//
private:
	server* Socket;
	bool				bIsConnected;	// 서버와 접속 유무
	int					SessionId;		// 캐릭터의 세션 고유 아이디 (랜덤값)
	cPlayerInfo *		CharactersInfo;	// 다른 캐릭터의 정보

	void SendPlayerInfo();		// 플레이어 위치 송신
	bool UpdateWorldInfo();		// 월드 동기화
	void UpdatePlayerInfo(const cPlayer & info);		// 플레이어 동기화	

	FTimerHandle SendPlayerInfoHandle;	// 동기화 타이머 핸들러

	// 새 플레이어 입장
	int	nPlayers;
	bool bNewPlayerEntered;
	cPlayer * NewPlayer;
	void UpdateNewPlayer();
	
	
};
