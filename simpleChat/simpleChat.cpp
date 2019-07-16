// simpleChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
	
#define QUEUE_LENGTH 20

using namespace std;

class simpleChat
{
	
};

int main(int argc, char ** argv)
{
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址

	PCSTR servIP = "127.0.0.1";
	if (argc>=3)
	{
		servIP = argv[2];
	}
	int tmpRes = inet_pton(sockAddr.sin_family,servIP,&sockAddr.sin_addr.s_addr); //inet_addr("127.0.0.1");  //具体的IP地址
	if (tmpRes != 1)
	{
		cout << tmpRes << endl;
		return -1;
	}

	int servPort = 1234;
	int tmpInputPort = 0;
	if (argc>=4)
	{
		tmpInputPort = strtol(argv[3], NULL, 10);
	}
	if (tmpInputPort > 0 && tmpInputPort < 65536)
	{
		servPort = tmpInputPort;
	}
	sockAddr.sin_port = htons(servPort);  //端口

	if (argc>=2 && argv[1][0] == 'c')
	{
		connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

		char szBuffer[MAXBYTE] = { 0 };
		recv(sock, szBuffer, MAXBYTE, NULL);

		printf("[Server]: %s\n", szBuffer);

		cout << "Exiting server." << endl;
		closesocket(sock);

		WSACleanup();

		return 0;
	}

	bind(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//进入监听状态
	listen(sock, QUEUE_LENGTH);
	cout << "Listening on " << servIP <<":" << servPort <<endl;

	//接收客户端请求
	SOCKADDR s_addrClnt;
	int nSize = sizeof(SOCKADDR);
	SOCKET socClnt = accept(sock, (SOCKADDR*)&s_addrClnt, &nSize);

	//向客户端发送数据
	const char *str = "Conneted to Server.";
	send(socClnt, str, strlen(str) + sizeof(char), NULL);

	//关闭套接字
	cout << "Server terminating." << endl;
	closesocket(socClnt);
	closesocket(sock);

	//终止 DLL 的使用
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
