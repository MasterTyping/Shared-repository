#include "pch.h"
#include "Server.h"
#include"Iocp.h"
#include<sstream>
#include<process.h>
#pragma warning (disable : 4996)

PlayerInfo		Server::playerinfo;
DWORD	recvBytes = NULL;
DWORD	sendBytes = NULL;

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	Server* pOverlappedEvent = (Server*)p;
	pOverlappedEvent->WorkerThreadFunc();
	return 0;
}

Server::Server()
{
	// 멤버 변수 초기화
	WorkerThread = true;
	Accept = true;

	for (int i = 0; i < MAX_USER; i++)
	{
		playerinfo.WorldCharacterInfo[i].ClientID = -1;
		playerinfo.WorldCharacterInfo[i].X = -1;
		playerinfo.WorldCharacterInfo[i].Y = -1;
		playerinfo.WorldCharacterInfo[i].Z = -1;
		playerinfo.WorldCharacterInfo[i].Pitch = 0;
		playerinfo.WorldCharacterInfo[i].Roll = 0;
		playerinfo.WorldCharacterInfo[i].Yaw = 0;
	}
}

//소멸자
Server::~Server()
{
	// winsock 의 사용을 끝낸다
	WSACleanup();
	// 다 사용한 객체를 삭제
	if (SocketInfo)
	{
		delete[] SocketInfo;
		SocketInfo = NULL;
	}

	if (WorkerHandle)
	{
		delete[] WorkerHandle;
		WorkerHandle = NULL;
	}
}

bool Server::Initialize()
{
	WSADATA wsaData;
	int nResult;
	// winsock 2.2 버전으로 초기화
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0)
	{
		printf("[ERROR] winsock 초기화 실패\n");
		return false;
	}

	// 소켓 생성
	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("[ERROR] 소켓 생성 실패\n");
		return false;
	}

	// 서버 정보 설정
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVERPROT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 소켓 설정
	// boost bind 와 구별짓기 위해 ::bind 사용
	nResult = ::bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nResult == SOCKET_ERROR)
	{
		printf("[ERROR] bind 실패\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// 수신 대기열 생성
	nResult = listen(ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		printf("[ERROR] listen 실패\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void Server::StartServer()
{
	int nResult;
	// 클라이언트 정보
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET clientSocket;
	DWORD recvBytes;
	DWORD flags;

	// Completion Port 객체 생성
	Iocp = CreateIoCompletionPort
	(
		INVALID_HANDLE_VALUE, 
		NULL, 
		0, 
		0
	);

	// Worker Thread 생성
	if (!CreateWorkerThread())
	{
		return;
	}
	printf("[INFO] 서버 시작...\n");

	// 클라이언트 접속을 받음
	while (Accept)
	{
		clientSocket = WSAAccept
		(
			ListenSocket,
			(struct sockaddr *)&clientAddr,
			&addrLen,
			NULL,
			NULL
		);

		if (clientSocket == INVALID_SOCKET)
		{
			printf("[ERROR] Accept 실패\n");
			return;
		}

		SocketInfo = new SOCKETINFO();
		SocketInfo->socket = clientSocket;
		SocketInfo->recvBytes = 0;
		SocketInfo->sendBytes = 0;
		SocketInfo->dataBuf.len = SOCKET_BUF_SIZE;
		SocketInfo->dataBuf.buf = SocketInfo->messageBuffer;
		flags = 0;

		Iocp = CreateIoCompletionPort
		(
			(HANDLE)clientSocket, 
			Iocp,
			(DWORD)SocketInfo, 
			0
		);

		// 중첩 소켓을 지정하고 완료시 실행될 함수를 넘겨줌
		nResult = WSARecv
		(
			SocketInfo->socket,
			&SocketInfo->dataBuf,
			1,
			&recvBytes,
			&flags,
			&(SocketInfo->overlapped),
			NULL
		);

		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("[ERROR] IO Pending 실패 : %d", WSAGetLastError());
			return;
		}
	}

}

bool Server::CreateWorkerThread()
{
	unsigned int threadId;
	// 시스템 정보 가져옴
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("[INFO] CPU 갯수 : %d\n", sysInfo.dwNumberOfProcessors);
	// 적절한 작업 스레드의 갯수는 (CPU * 2) + 1
	int nThreadCnt = sysInfo.dwNumberOfProcessors * 2;

	// thread handler 선언
	WorkerHandle = new HANDLE[nThreadCnt];
	// thread 생성
	for (int i = 0; i < nThreadCnt; i++)
	{
		WorkerHandle[i] = (HANDLE *)_beginthreadex
		(
			NULL,
			0,
			&CallWorkerThread, 
			this, 
			CREATE_SUSPENDED, 
			&threadId
		);
		if (WorkerHandle[i] == NULL)
		{
			printf("[ERROR] Worker Thread 생성 실패\n");
			return false;
		}
		ResumeThread(WorkerHandle[i]);
	}
	return true;
}

void Server::WorkerThreadFunc()
{
	// 함수 호출 성공 여부
	BOOL	Result;
	int		SucResult;
	// Completion Key를 받을 포인터 변수
	SOCKETINFO *	pCompletionKey;
	// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터	
	SOCKETINFO *	pSocketInfo;

	DWORD	dwFlags = 0;

	while (WorkerThread)
	{
		/**
		 * 이 함수로 인해 쓰레드들은 WaitingThread Queue 에 대기상태로 들어가게 됨
		 * 완료된 Overlapped I/O 작업이 발생하면 IOCP Queue 에서 완료된 작업을 가져와
		 * 뒷처리를 함
		 */
		Result = GetQueuedCompletionStatus
		(
			Iocp,
			&recvBytes,				// 실제로 전송된 바이트
			(PULONG_PTR)&pCompletionKey,	// completion key
			(LPOVERLAPPED *)&pSocketInfo,			// overlapped I/O 객체
			INFINITE				// 대기할 시간
		);

		if (!Result && recvBytes == 0)
		{
			printf("[INFO] socket(%d) 접속 끊김\n", pSocketInfo->socket);
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		pSocketInfo->dataBuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}
		else
		{
			stringstream RecvStream;
			Player info;
			RecvStream << pSocketInfo->dataBuf.buf;
			RecvStream >> info;

			printf("[INFO] [%d]정보 수신 - X : [%f], Y : [%f], Z : [%f], Yaw : [%f], Roll : [%f], Pitch : [%f]\n",
				info.ClientID, info.X, info.Y, info.Z, info.Yaw, info.Roll, info.Pitch);

			cout << "위치 저장, 회전값 저장 부분 시작 전" << endl;
			// 캐릭터의 위치를 저장						
			playerinfo.WorldCharacterInfo[info.ClientID].ClientID = info.ClientID;
			playerinfo.WorldCharacterInfo[info.ClientID].X = info.X;
			playerinfo.WorldCharacterInfo[info.ClientID].Y = info.Y;
			playerinfo.WorldCharacterInfo[info.ClientID].Z = info.Z;
			// 캐릭터의 회전값을 저장
			playerinfo.WorldCharacterInfo[info.ClientID].Yaw = info.Yaw;
			playerinfo.WorldCharacterInfo[info.ClientID].Pitch = info.Pitch;
			playerinfo.WorldCharacterInfo[info.ClientID].Roll = info.Roll;

			// 직렬화
			stringstream SendStream;
			SendStream << playerinfo;

			//data.buf 에다 직접 데이터를 쓰면 쓰레기값이 전달될 수 있음
			CopyMemory
			(
				pSocketInfo->messageBuffer,
				(CHAR*)SendStream.str().c_str(),
				SendStream.str().length()
			);
			pSocketInfo->dataBuf.buf = pSocketInfo->messageBuffer;
			pSocketInfo->dataBuf.len = SendStream.str().length();

			// 다른 클라이언트의 정보를 송신
			SucResult = WSASend
			(
				pSocketInfo->socket,
				&(pSocketInfo->dataBuf),
				1,
				&sendBytes,
				dwFlags,
				NULL,
				NULL
			);

			if (SucResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("[ERROR] WSASend 실패 : ", WSAGetLastError());
			}

			// stSOCKETINFO 데이터 초기화
			ZeroMemory(&(pSocketInfo->overlapped), sizeof(OVERLAPPED));
			ZeroMemory(pSocketInfo->messageBuffer, SOCKET_BUF_SIZE);
			pSocketInfo->dataBuf.len = SOCKET_BUF_SIZE;
			pSocketInfo->dataBuf.buf = pSocketInfo->messageBuffer;
			pSocketInfo->recvBytes = 0;
			pSocketInfo->sendBytes = 0;

			dwFlags = 0;

			// 클라이언트로부터 다시 응답을 받기 위해 WSARecv 를 호출해줌
			SucResult = WSARecv
			(
				pSocketInfo->socket,
				&(pSocketInfo->dataBuf),
				1,
				&recvBytes,
				&dwFlags,
				(LPWSAOVERLAPPED)&(pSocketInfo->overlapped),
				NULL
			);

			if (SucResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("[ERROR] WSARecv 실패 : ", WSAGetLastError());
			}
		}
	}
}
