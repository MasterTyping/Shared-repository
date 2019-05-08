#pragma once
#include"pch.h"
// ��Ƽ����Ʈ ���� ���� define
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<winsock.h>
// DB����
// DBconnector Ŭ������ ����
#define DB_ADDRESS		"localhost"
#define DB_PORT			 3306
#define DB_ID			"root"
#define DB_PASSWORD		"rhrnak"
#define DB_TITLE		"graduation"

using namespace std;

// IOCP ���� ����ü
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
	//���̵�
	int ClientID;

	// ��ġ
	float X;
	float Y;
	float Z;
	// ȸ����
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

	// ���� ��� �� ���� ���� ����
	bool Initialize();
	// ���� ����
	void StartServer();
	// �۾� ������ ����
	bool CreateWorkerThread();
	// �۾� ������
	void WorkerThreadFunc();

private:
	SOCKETINFO *	SocketInfo;		// ���� ����
	SOCKET			ListenSocket;		// ���� ���� ����
	HANDLE			Iocp;			// IOCP ��ü �ڵ�
	bool			Accept;			// ��û ���� �÷���
	bool			WorkerThread;	// �۾� ������ ���� �÷���
	HANDLE *		WorkerHandle;	// �۾� ������ �ڵ�	
	static PlayerInfo playerinfo;
};
