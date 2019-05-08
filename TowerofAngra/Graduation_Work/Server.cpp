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
	// ��� ���� �ʱ�ȭ
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

//�Ҹ���
Server::~Server()
{
	// winsock �� ����� ������
	WSACleanup();
	// �� ����� ��ü�� ����
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
	// winsock 2.2 �������� �ʱ�ȭ
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (nResult != 0)
	{
		printf("[ERROR] winsock �ʱ�ȭ ����\n");
		return false;
	}

	// ���� ����
	ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("[ERROR] ���� ���� ����\n");
		return false;
	}

	// ���� ���� ����
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVERPROT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// ���� ����
	// boost bind �� �������� ���� ::bind ���
	nResult = ::bind(ListenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nResult == SOCKET_ERROR)
	{
		printf("[ERROR] bind ����\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// ���� ��⿭ ����
	nResult = listen(ListenSocket, 5);
	if (nResult == SOCKET_ERROR)
	{
		printf("[ERROR] listen ����\n");
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	return true;
}

void Server::StartServer()
{
	int nResult;
	// Ŭ���̾�Ʈ ����
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET clientSocket;
	DWORD recvBytes;
	DWORD flags;

	// Completion Port ��ü ����
	Iocp = CreateIoCompletionPort
	(
		INVALID_HANDLE_VALUE, 
		NULL, 
		0, 
		0
	);

	// Worker Thread ����
	if (!CreateWorkerThread())
	{
		return;
	}
	printf("[INFO] ���� ����...\n");

	// Ŭ���̾�Ʈ ������ ����
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
			printf("[ERROR] Accept ����\n");
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

		// ��ø ������ �����ϰ� �Ϸ�� ����� �Լ��� �Ѱ���
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
			printf("[ERROR] IO Pending ���� : %d", WSAGetLastError());
			return;
		}
	}

}

bool Server::CreateWorkerThread()
{
	unsigned int threadId;
	// �ý��� ���� ������
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf("[INFO] CPU ���� : %d\n", sysInfo.dwNumberOfProcessors);
	// ������ �۾� �������� ������ (CPU * 2) + 1
	int nThreadCnt = sysInfo.dwNumberOfProcessors * 2;

	// thread handler ����
	WorkerHandle = new HANDLE[nThreadCnt];
	// thread ����
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
			printf("[ERROR] Worker Thread ���� ����\n");
			return false;
		}
		ResumeThread(WorkerHandle[i]);
	}
	return true;
}

void Server::WorkerThreadFunc()
{
	// �Լ� ȣ�� ���� ����
	BOOL	Result;
	int		SucResult;
	// Completion Key�� ���� ������ ����
	SOCKETINFO *	pCompletionKey;
	// I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������	
	SOCKETINFO *	pSocketInfo;

	DWORD	dwFlags = 0;

	while (WorkerThread)
	{
		/**
		 * �� �Լ��� ���� ��������� WaitingThread Queue �� �����·� ���� ��
		 * �Ϸ�� Overlapped I/O �۾��� �߻��ϸ� IOCP Queue ���� �Ϸ�� �۾��� ������
		 * ��ó���� ��
		 */
		Result = GetQueuedCompletionStatus
		(
			Iocp,
			&recvBytes,				// ������ ���۵� ����Ʈ
			(PULONG_PTR)&pCompletionKey,	// completion key
			(LPOVERLAPPED *)&pSocketInfo,			// overlapped I/O ��ü
			INFINITE				// ����� �ð�
		);

		if (!Result && recvBytes == 0)
		{
			printf("[INFO] socket(%d) ���� ����\n", pSocketInfo->socket);
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

			printf("[INFO] [%d]���� ���� - X : [%f], Y : [%f], Z : [%f], Yaw : [%f], Roll : [%f], Pitch : [%f]\n",
				info.ClientID, info.X, info.Y, info.Z, info.Yaw, info.Roll, info.Pitch);

			cout << "��ġ ����, ȸ���� ���� �κ� ���� ��" << endl;
			// ĳ������ ��ġ�� ����						
			playerinfo.WorldCharacterInfo[info.ClientID].ClientID = info.ClientID;
			playerinfo.WorldCharacterInfo[info.ClientID].X = info.X;
			playerinfo.WorldCharacterInfo[info.ClientID].Y = info.Y;
			playerinfo.WorldCharacterInfo[info.ClientID].Z = info.Z;
			// ĳ������ ȸ������ ����
			playerinfo.WorldCharacterInfo[info.ClientID].Yaw = info.Yaw;
			playerinfo.WorldCharacterInfo[info.ClientID].Pitch = info.Pitch;
			playerinfo.WorldCharacterInfo[info.ClientID].Roll = info.Roll;

			// ����ȭ
			stringstream SendStream;
			SendStream << playerinfo;

			//data.buf ���� ���� �����͸� ���� �����Ⱚ�� ���޵� �� ����
			CopyMemory
			(
				pSocketInfo->messageBuffer,
				(CHAR*)SendStream.str().c_str(),
				SendStream.str().length()
			);
			pSocketInfo->dataBuf.buf = pSocketInfo->messageBuffer;
			pSocketInfo->dataBuf.len = SendStream.str().length();

			// �ٸ� Ŭ���̾�Ʈ�� ������ �۽�
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
				printf("[ERROR] WSASend ���� : ", WSAGetLastError());
			}

			// stSOCKETINFO ������ �ʱ�ȭ
			ZeroMemory(&(pSocketInfo->overlapped), sizeof(OVERLAPPED));
			ZeroMemory(pSocketInfo->messageBuffer, SOCKET_BUF_SIZE);
			pSocketInfo->dataBuf.len = SOCKET_BUF_SIZE;
			pSocketInfo->dataBuf.buf = pSocketInfo->messageBuffer;
			pSocketInfo->recvBytes = 0;
			pSocketInfo->sendBytes = 0;

			dwFlags = 0;

			// Ŭ���̾�Ʈ�κ��� �ٽ� ������ �ޱ� ���� WSARecv �� ȣ������
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
				printf("[ERROR] WSARecv ���� : ", WSAGetLastError());
			}
		}
	}
}
