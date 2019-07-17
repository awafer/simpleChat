// simpleChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
	
constexpr auto QUEUE_LENGTH = 20;
constexpr auto DEFAULT_IP = "127.0.0.1";
constexpr auto DEFAULT_PORT = 1234;

using namespace std;

class simpleChat
{
private:
	WSADATA wsaData;
	SOCKET sock;
	SOCKET socClnt;
	sockaddr_in sockAddr;
	SOCKADDR s_addrClnt;
	bool bClient=false;

private:
	void init()
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET;
		loadIPandPort(DEFAULT_IP, DEFAULT_PORT);
	}
	void loadIPandPort(const char * szIP, int port)
	{
		int tmpRes = inet_pton(sockAddr.sin_family, szIP, &sockAddr.sin_addr.s_addr); //inet_addr("127.0.0.1");  //�����IP��ַ
		//cout << _servIP << ":" << sockAddr.sin_addr.s_addr << endl;
		//cout << tmpRes << endl;
		sockAddr.sin_port = htons(port);
		//cout << _servPort<<":" << ntohs(sockAddr.sin_port) << endl;
	}

public:
	void bindSocket()
	{
		bind(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	}
	
	void listenSocket()
	{
		listen(sock, QUEUE_LENGTH);
		char __tmpIP[INET_ADDRSTRLEN];
		const char * __ptmpIP=inet_ntop(PF_INET, &sockAddr.sin_addr, __tmpIP, sizeof(__tmpIP));
		if (__ptmpIP == NULL)
		{
			cout << "Something wrong." << endl;
		}
		cout << "Listening on " << __tmpIP << ":" << ntohs(sockAddr.sin_port) << endl;
	}

	void acceptSocket()
	{
		int nSize = sizeof(SOCKADDR);
		socClnt = accept(sock, (SOCKADDR*)&s_addrClnt, &nSize);
	}

	void connectServer()
	{
		if (bClient)
		{
			int res = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
			char __tmpIP[INET_ADDRSTRLEN];
			const char * __ptmpIP = inet_ntop(PF_INET, &sockAddr.sin_addr, __tmpIP, sizeof(__tmpIP));
			if (__ptmpIP == NULL)
			{
				cout << "Something wrong." << endl;
			}
			//cout << "Connected to " << __tmpIP << ":" << ntohs(sockAddr.sin_port) << endl;
		}
		else
		{
			cout << "Not a client." << endl;
		}
	}

	//��ͻ��˷�������
	void sendMessage()
	{
		const char *str = "Conneted to Server.";
		send(socClnt, str, strlen(str) + sizeof(char), NULL);
	}

	void sendMessage(const char * msg)
	{

	}

	void recvMessage()
	{
		char szBuffer[MAXBYTE] = { 0 };
		recv(sock, szBuffer, MAXBYTE, NULL);
		cout<<"[Server]:" << szBuffer <<endl;
	}
	
	void closeSocket()
	{
		//�ر��׽���
		if (!bClient)
		{
			cout << "Server terminating." << endl;
			closesocket(socClnt);
		}
		else
		{
			cout << "Exiting server." << endl;
		}
		closesocket(sock);

		//��ֹ DLL ��ʹ��
		WSACleanup();
	}

	simpleChat()
	{
		init();
		//cout << "Here" << endl;
		//PCSTR _servIP = "127.0.0.1";
		//int tmpRes = inet_pton(sockAddr.sin_family, _servIP, &sockAddr.sin_addr.s_addr); //inet_addr("127.0.0.1");  //�����IP��ַ
		//cout << _servIP << ":" << sockAddr.sin_addr.s_addr << endl;
		//cout << tmpRes << endl;
		//int _servPort = 1234;
		//sockAddr.sin_port = htons(_servPort);
		//cout << _servPort<<":" << ntohs(sockAddr.sin_port) << endl;
		//loadIPandPort(_servIP, _servPort);
	}

	simpleChat(const char * szIP, int port)
	{
		init();
		loadIPandPort(szIP, port);
	}

	simpleChat(bool bclient)
	{
		bClient = bclient;
		init();
		//loadIPandPort("127.0.0.1", 1234);
		//cout << "bClient"<<":"<<bClient << endl;
		//cout <<  "IP:" << sockAddr.sin_addr.s_addr << endl;
		//cout << "Port:" << ntohs(sockAddr.sin_port) << endl;
	}

	simpleChat(bool bclient, const char * szIP, int port)
	{
		bClient = bclient;
		init();
		loadIPandPort(szIP, port);
	}
};

int main(int argc, char ** argv)
{
	PCSTR tmpIP=DEFAULT_IP;
	int tmpPort = DEFAULT_PORT;

	if (argc >= 2 && argv[1][0] == 'c')
	{
		if (argc >= 3)
		{
			tmpIP = argv[2];
		}
		if (argc >= 4)
		{
			tmpPort = strtol(argv[3], NULL, 10);
		}
		simpleChat sc(true, tmpIP, tmpPort);
		sc.connectServer();
		sc.recvMessage();
		sc.closeSocket();
		return 0;
	}

	if (argc == 3)
	{
		tmpIP = argv[1];
		tmpPort = strtol(argv[2], NULL, 10);
		simpleChat sc(tmpIP, tmpPort);
		sc.bindSocket();
		sc.listenSocket();
		sc.acceptSocket();
		sc.sendMessage();
		sc.closeSocket();
	}
	//��ʼ�� DLL
	//WSADATA wsaData;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);

	//�����׽���
	//SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//���׽���
	//sockaddr_in sockAddr;
	//memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	//sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ

	//PCSTR servIP = "127.0.0.1";
	//if (argc>=3)
	//{
	//	servIP = argv[2];
	//}
	//int tmpRes = inet_pton(sockAddr.sin_family,servIP,&sockAddr.sin_addr.s_addr); //inet_addr("127.0.0.1");  //�����IP��ַ
	/*if (tmpRes != 1)
	{
		cout << tmpRes << endl;
		return -1;
	}*/

	//int servPort = 1234;
	/*int tmpInputPort = 0;
	if (argc>=4)
	{
		tmpInputPort = strtol(argv[3], NULL, 10);
	}
	if (tmpInputPort > 0 && tmpInputPort < 65536)
	{
		servPort = tmpInputPort;
	}*/
	//sockAddr.sin_port = htons(servPort);  //�˿�
	/*
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
	*/

	//bind(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

	//�������״̬
	//listen(sock, QUEUE_LENGTH);
	//cout << "Listening on " << servIP <<":" << servPort <<endl;

	//���տͻ�������
	//SOCKADDR s_addrClnt;
	//int nSize = sizeof(SOCKADDR);
	//SOCKET socClnt = accept(sock, (SOCKADDR*)&s_addrClnt, &nSize);

	//��ͻ��˷�������
	//const char *str = "Conneted to Server.";
	//send(socClnt, str, strlen(str) + sizeof(char), NULL);

	//�ر��׽���
	//cout << "Server terminating." << endl;
	//closesocket(socClnt);
	//closesocket(sock);

	//��ֹ DLL ��ʹ��
	//WSACleanup();

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
