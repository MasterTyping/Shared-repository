#pragma once
// Ŭ���̾�Ʈ�� ���� ���� ��ſ� �ʿ��� �������� define ���� -> Ŭ�� �϶���  sc_ , Ŭ�� -> ���� �� ���� cs_ �̷������� �ڵ� .
// �𸮾� �������� �߰��Ҷ� ��� �̿��ؼ� ������� �߰�.
//.

#include <iostream>
#include<cstdio>
#include<time.h>
#include<ctime>
#include<algorithm>
#include<tchar.h>
#include<WinSock2.h>
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<thread>
#include<vector>
#include<mutex>
#include<array>
#include<chrono>
#include<tchar.h>
#include<list>
#include<map>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>


//Network
#define SOCKET_BUF_SIZE			4096
#define SERVERPROT				9000
#define SERVERIP				"127.0.0.1"
#define WORKER_THREAD_NUMBER	4

#define MAX_USER				4			//�ϴ��� �ִ��ο�
#define MAX_MAP					7			//��������



//Client to Server
#define CS_KEY			1
#define CS_COLLISION	2
#define CS_DAMAGE		3


//
//-----OVERLAP_STATE-----
#define OP_RECV					1
#define OP_SEND					2
//

using namespace DirectX;
using namespace DirectX::PackedVector;
#pragma comment(lib, "d3d11")
#include "Math.h"


typedef struct PLAYER 
{			
	Vec3			m_vPosition;
	Vec3			m_vDir;
	FLOAT			m_fAngle;
	Vec3			m_vAngle;
	Vec3			m_vAxis[3];
	float			m_fAxisLength[3];
} PLAYER;

typedef struct OverlapEx
{
	WSAOVERLAPPED	original_overlap;
	int				operation;
	WSABUF			recv_buff;
	unsigned char	socket_buff[SOCKET_BUF_SIZE];
	int				packet_size;
	BYTE			event_type;
	int				event_target;
	int				ID;
	BYTE			MapType;
}OverlapEx;

typedef struct CLIENT
{
	bool			is_Connected;
	bool			is_Gaming;
	int				ID;
	int				StageNumber;
	int				Speed;
	int				previous_data_size;
	Vec3			Character_Direction;	//ĳ������ ����
	PLAYER			Player;
	SOCKET			sock;
	int	StageList;
	OverlapEx		recv_overlap;
}CLIENT;