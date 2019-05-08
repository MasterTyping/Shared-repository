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

	// ���� ���̵� ��ȭ
	UFUNCTION(BlueprintPure, Category = "Properties")
		int GetSessionId();

	// ������ų �ٸ� ĳ����
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACharacter> WhoToSpawn;
	//
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ���Ǿ��̵� ��Ī�Ǵ� ���� ��ȯ
	AActor* FindActorBySessionId(TArray<AActor*> ActorArray, const int& SessionId);

	// �������κ��� ���� ���� ����
	void RecvWorldInfo(cPlayerInfo * ci);

	// �� �÷��̾� ������Ʈ
	void RecvNewPlayer(cPlayer * NewPlayer);
	//
	//
private:
	server* Socket;
	bool				bIsConnected;	// ������ ���� ����
	int					SessionId;		// ĳ������ ���� ���� ���̵� (������)
	cPlayerInfo *		CharactersInfo;	// �ٸ� ĳ������ ����

	void SendPlayerInfo();		// �÷��̾� ��ġ �۽�
	bool UpdateWorldInfo();		// ���� ����ȭ
	void UpdatePlayerInfo(const cPlayer & info);		// �÷��̾� ����ȭ	

	FTimerHandle SendPlayerInfoHandle;	// ����ȭ Ÿ�̸� �ڵ鷯

	// �� �÷��̾� ����
	int	nPlayers;
	bool bNewPlayerEntered;
	cPlayer * NewPlayer;
	void UpdateNewPlayer();
	
	
};
