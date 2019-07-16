// simpleChatClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

int main(int argc, char ** argv) {
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;

	PCSTR servIP = "127.0.0.1";
	if (argv[1] != NULL)
	{
		servIP = argv[1];
	}

	int tmpRes = inet_pton(sockAddr.sin_family, argv[1], &sockAddr.sin_addr.s_addr); //inet_addr("127.0.0.1");  //具体的IP地址
	if (tmpRes != 1)
	{
		return -1;
	}

	int servPort = 1234;
	int tmpInputPort = 0;
	if (argv[2] != NULL)
	{
		tmpInputPort = strtol(argv[2], NULL, 10);
	}
	if (tmpInputPort > 0 && tmpInputPort < 65536)
	{
		servPort = tmpInputPort;
	}

	sockAddr.sin_port = htons(servPort);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//接收服务器传回的数据
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);

	//输出接收到的数据
	printf("[Server]: %s\n", szBuffer);

	//关闭套接字
	closesocket(sock);

	//终止使用 DLL
	WSACleanup();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
