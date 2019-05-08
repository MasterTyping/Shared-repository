// Fill out your copyright notice in the Description page of Project Settings.
#pragma warning(disable : 4996)
#pragma once
#include "server.h"
#include"Runtime/Core/Public/HAL/Runnable.h"
#include<string>
#include<sstream>
#include<algorithm>


server::server()
{
}


server::~server()
{
	closesocket(Sock);
	WSACleanup();
}


int server::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}
bool server::InitSock()
{
	WSADATA wsaData;
	// 윈속 버전을 2.2로 초기화
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0)
	{
		return false;
	}

	// TCP 소켓 생성	
	Sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Sock == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}
bool server::Connect(const char*, int)
{
	// 접속할 서버 정보를 저장할 구조체
	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// 접속할 서버 포트 및 IP
	stServerAddr.sin_port = htons(SERVERPROT);
	stServerAddr.sin_addr.s_addr = inet_addr(SERVERIP);

	int nRet = connect(Sock, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}
void server::err_display(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void server::err_quit(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}


//FRunnable override 함수
bool server::Init()
{


	return true;
}
uint32 server::Run()
{


	return 0;
}
void server::Stop()
{

}
void server::Exit()
{

}
//FRunnable override 함수


cPlayerInfo* server::SyncCharacters(cPlayer& target)
{
	// 캐릭터 정보 직렬화
	stringstream InputStream;
	//	InputStream << PacketType::SEND_CHARACTER << endl;;
	InputStream << target;

	// 캐릭터 정보 전송
	int nSendLen = send
	(
		Sock,
		(CHAR*)InputStream.str().c_str(),
		InputStream.str().length(),
		0
	);

	if (nSendLen == -1)
	{
		return nullptr;
	}

	// 서버응답 수신
	int nRecvLen = recv
	(
		Sock,
		(CHAR*)&recvBuffer,
		SOCKET_BUF_SIZE,
		0
	);

	if (nRecvLen == -1)
	{
		return nullptr;
	}
	else
	{
		// 역직렬화
		stringstream OutputStream;
		OutputStream << recvBuffer;
		OutputStream >> playerinfo;

		return &playerinfo;
	}
}

void server::LogoutCharacter(int SessionId)
{
	stringstream SendStream;
	SendStream << PacketType::LOGOUT_CHARACTER << endl;
	SendStream << SessionId << endl;

	int nSendLen = send
	(
		Sock,
		(CHAR*)SendStream.str().c_str(),
		SendStream.str().length(),
		0
	);

	if (nSendLen == -1)
	{
		return;
	}

	closesocket(Sock);
	WSACleanup();
}