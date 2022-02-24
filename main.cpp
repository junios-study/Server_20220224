#include <WinSock2.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSAData wsaData;

	//winsock 초기화, 윈도우만 합니다. linux, unix에선 사용하지 않아요.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() <<  endl;
		exit(-1);
	}
}