// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
// *******************************************************
#define WIN32_LEAN_AND_MEAN
// *******************************************************
#include "CoreMinimal.h"
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include<iostream>
#include<map>
#include<vector>
#include<mutex>
#include"Runtime/Core/Public/HAL/Runnable.h"

class AprojectlevelGameMode;

using namespace std;

//Network Define
#define SOCKET_BUF_SIZE			4096
#define SERVERPROT				9000
#define SERVERIP				"127.0.0.1"

// 소켓 통신 구조체
struct SOCKETIONFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[SOCKET_BUF_SIZE];
	int				recvBytes;
	int				sendBytes;
};

// 주고받을 패킷 타입.
enum Packet
{
	PLAYER_BASICATTACK,
	PLAYER_SKILLATTACK,
	PLAYER_DAMAGE,
	MONSTER_DAMAGE,
	MONSTER_ATTACK,
	SPAWN_MONSTER,
	MONSTER_DIE,
	PLAYER_DIE,
	END
};

class cPlayer
{
public:
	cPlayer() {};
	~cPlayer() {};

private:


public:
	//아이디
	int ClientID = 0;

	// 위치
	float X;
	float Y;
	float Z;
	// 회전값
	float Yaw;
	float Pitch;
	float Roll;

	//attribute
	bool now_live;
	float now_hp;
	bool now_attack;


	//패킷 직렬화 역직렬화
	friend ostream& operator<<(ostream &stream, cPlayer& info)
	{
		stream << info.ClientID << endl;
		stream << info.X << endl;
		stream << info.Y << endl;
		stream << info.Z << endl;
		stream << info.Yaw << endl;
		stream << info.Pitch << endl;
		stream << info.Roll << endl;

		return stream;
	}

	friend istream& operator>>(istream& stream, cPlayer& info)
	{
		stream >> info.ClientID;
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;
		stream >> info.Yaw;
		stream >> info.Pitch;
		stream >> info.Roll;

		return stream;
	}
};

// 서버파일 include protocol file 하면 4로 define 되있는거 MAX_USER변경
class cPlayerInfo
{
public:
	cPlayerInfo() {};
	~cPlayerInfo() {};

	cPlayer WorldCharacterInfo[100];

	friend ostream& operator<<(ostream &stream, cPlayerInfo& info)
	{
		for (int i = 0; i < 100; i++)
		{
			stream << info.WorldCharacterInfo[i] << endl;
		}
		return stream;
	}

	friend istream &operator>>(istream &stream, cPlayerInfo& info)
	{
		for (int i = 0; i < 100; i++)
		{
			stream >> info.WorldCharacterInfo[i];
		}
		return stream;
	}
};
enum PacketType
{
	SEND_CHARACTER,
	LOGOUT_CHARACTER
};

class PROJECTLEVEL_API server : public FRunnable
{
private:
	SOCKET	Sock;						// 서버와 연결할 소켓	
	cPlayerInfo playerinfo;				// 케릭터 정보
	char 	recvBuffer[SOCKET_BUF_SIZE];		// 수신 버퍼 스트림

public:
	server();
	virtual ~server();

public:
	int nTotalSockets = 0;
	SOCKETIONFO* SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];


	//소켓 초기화
	bool InitSock();
	bool Connect(const char*, int);
	//오류 출력 함수
	void err_quit(const char* msg);
	void err_display(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);
	//

	// FRunnable Thread members	
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	//FRunnable override 함수
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
	//======================

	cPlayerInfo* SyncCharacters(cPlayer& target);
	void LogoutCharacter(int SessionId);

	// 싱글턴 객체 가져오기
	static server* GetSingleton()
	{
		static server ins;
		return &ins;
	}

};
