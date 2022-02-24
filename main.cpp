#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;

	//1.winsock 초기화, 윈도우만 합니다. linux, unix에선 사용하지 않아요.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() <<  endl;
		exit(-1);
	}

	//2.create socket
	SOCKET ServerSocket;//listen 클라이언트 대기 소켓(문)

	ServerSocket = socket(
		AF_INET,
		SOCK_STREAM,
		0
	);

	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "socket Error : " << GetLastError() << endl;
		exit(-1);
	}

	//2. ip, port를 할당해서 소켓이랑 연결
	SOCKADDR_IN ServerAddr;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(50000);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind Error : " << GetLastError() << endl;
		exit(-1);
	}

	//3. 클라이언트 연결을 기다려라.
	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError() << endl;
		exit(-1);
	}

	while (1)
	{
		//4. 수락해서 클라이언트와 연결을 완성
		SOCKADDR_IN ClientAddr;
		int ClinetAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket; //클라이언트 된 통로(문)
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClinetAddrSize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError() << endl;
			exit(-1);
		}

		//network to host short
		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr) << endl;
		cout << "connect port : " << ntohs(ClientAddr.sin_port) << endl;

		//5. 보낸다 자료를 클라이언트한테
		char Message[] = "Hello World"; //'\0'
		send(ClientSocket, Message, strlen(Message) + 1, 0);

		//6. 연결 종료
		closesocket(ClientSocket);
	}

	closesocket(ServerSocket);

	//7.Winsock 종료, 윈도우만 합니다.
	WSACleanup();

	return 0;
}