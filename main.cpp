#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib")



struct NumberPacket {
	int Number1;
	int Number2;
};

int main()
{
	WSAData wsaData;

	//1.winsock �ʱ�ȭ, �����츸 �մϴ�. linux, unix���� ������� �ʾƿ�.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() <<  endl;
		exit(-1);
	}

	//2.create socket
	SOCKET ServerSocket;//listen Ŭ���̾�Ʈ ��� ����(��)

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

	//2. ip, port�� �Ҵ��ؼ� �����̶� ����
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

	//3. Ŭ���̾�Ʈ ������ ��ٷ���.
	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError() << endl;
		exit(-1);
	}

	while (1)
	{
		//4. �����ؼ� Ŭ���̾�Ʈ�� ������ �ϼ�
		SOCKADDR_IN ClientAddr;
		int ClinetAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket; //Ŭ���̾�Ʈ �� ���(��)
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClinetAddrSize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError() << endl;
			exit(-1);
		}

		//inet network to address
		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr) << endl;
		//network to host short
		cout << "connect port : " << ntohs(ClientAddr.sin_port) << endl;

		//5. ������ �ڷḦ Ŭ���̾�Ʈ����
		NumberPacket Packet;
		recv(ClientSocket, (char*)(&Packet), 8, 0);

		cout << "client sended : " << Packet.Number1 << endl;
		cout << "client sended : " << Packet.Number2 << endl;

		int IntResult = Packet.Number1 + Packet.Number2;

		string Result = to_string(IntResult);

		send(ClientSocket, Result.c_str(), Result.length() + 1, 0);

		cout << "client send : " << Result << endl;

		//6. ���� ����
		closesocket(ClientSocket);
	}

	closesocket(ServerSocket);

	//7.Winsock ����, �����츸 �մϴ�.
	WSACleanup();

	return 0;
}