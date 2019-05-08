#pragma once
#include"pch.h"
// 멀티바이트 집합 사용시 define
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock.h>
// DB정보
// DBconnector 클래스랑 연결
#define DB_ADDRESS		"localhost"
#define DB_PORT			 3306
#define DB_ID			"root"
#define DB_PASSWORD		"rhrnak"
#define DB_TITLE		"graduation"

using namespace std;

// IOCP 소켓 구조체
struct SOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			messageBuffer[SOCKET_BUF_SIZE];
	int				recvBytes;
	int				sendBytes;
};



class Player
{
public:
	Player() {};
	~Player() {};

private:


public:
	//아이디
	int ClientID;

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

	friend ostream& operator<<(ostream &stream, Player& info)
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

	friend istream& operator>>(istream& stream, Player& info)
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

class PlayerInfo
{
public:
	PlayerInfo() {};
	~PlayerInfo() {};

	Player WorldCharacterInfo[MAX_USER];

	friend ostream& operator<<(ostream &stream, PlayerInfo& info)
	{
		for (int i = 0; i < MAX_USER; i++)
		{
			stream << info.WorldCharacterInfo[i] << endl;
		}
		return stream;
	}

	friend istream &operator>>(istream &stream, PlayerInfo& info)
	{
		for (int i = 0; i < MAX_USER; i++)
		{
			stream >> info.WorldCharacterInfo[i];
		}
		return stream;
	}
};

class Server: public singletonBase<Server>
{
public:
	Server();
	~Server();

	// 소켓 등록 및 서버 정보 설정
	bool Initialize();
	// 서버 시작
	void StartServer();
	// 작업 스레드 생성
	bool CreateWorkerThread();
	// 작업 스레드
	void WorkerThreadFunc();

private:
	SOCKETINFO *	SocketInfo;		// 소켓 정보
	SOCKET			ListenSocket;		// 서버 리슨 소켓
	HANDLE			Iocp;			// IOCP 객체 핸들
	bool			Accept;			// 요청 동작 플래그
	bool			WorkerThread;	// 작업 스레드 동작 플래그
	HANDLE *		WorkerHandle;	// 작업 스레드 핸들	
	static PlayerInfo playerinfo;
};
