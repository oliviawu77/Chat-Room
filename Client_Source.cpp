#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)
#include <WinSock2.h>
#include <string>
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNING


std::string acc, pwd;
SOCKET Connection;
void ClientThread()
{
	int dividingLinelenght; //���j�u����
	int bufferlength; //�T������
	int accountlenght; //�o�ܱb��W�r����

	//���w��T��
	//���T������
	recv(Connection, (char*)&bufferlength, sizeof(int), NULL);
	char * rcvMessage = new char[bufferlength + 1];
	rcvMessage[bufferlength] = '\0';
	//���T��
	recv(Connection, rcvMessage, bufferlength, NULL);
	std::cout << rcvMessage << std::endl;//��ܰT��(�w��T��)

	//�����b��W�r
	//���b�����
	recv(Connection, (char*)&accountlenght, sizeof(int), NULL);
	char * accountName = new char[accountlenght + 1];
	accountName[accountlenght] = '\0';
	//���b��
	recv(Connection, accountName, accountlenght, NULL);
	//��ܥثe�ϥΪ̬O��
	std::cout << accountName << std::endl; 


	//�����j�u
	//�����j�u����
	recv(Connection, (char*)&dividingLinelenght, sizeof(int), NULL);
	char * dividingLine = new char[dividingLinelenght + 1];
	dividingLine[dividingLinelenght + 1] = '\0';
	//�����j�u�r��
	recv(Connection, dividingLine, dividingLinelenght, NULL);
	//��ܤ��j�u
	std::cout << dividingLine << std::endl;


	//��"userlist"�r��
	int userListLenght;
	//��"userlist"����
	recv(Connection, (char*)&userListLenght, sizeof(int), NULL);
	char * userlist = new char[userListLenght + 1];
	userlist[userListLenght] = '\0';
	//��"userlist"�r��
	recv(Connection, (char*)userlist, userListLenght, NULL); 
	std::cout << userlist << std::endl;


	//��userlist
	int z;
	std::string liveAccoumt;
	for (z = 0; z < 10; z++)
	{
		int acclen;
		recv(Connection, (char*)&acclen, sizeof(int), NULL);
		char * liveAccoumt = new char[acclen + 1];
		liveAccoumt[acclen] = '\0';
		recv(Connection, (char*)liveAccoumt, acclen, NULL);
		std::cout << liveAccoumt << std::endl;
	}
	
	std::cout << dividingLine << std::endl;

	while (true)
	{
		//���o�ܱb��
		//���o�ܱb������
		recv(Connection, (char*)&accountlenght, sizeof(int), NULL); 
		char * accountName = new char[accountlenght + 1];
		accountName[accountlenght] = '\0';
		//���b��
		recv(Connection, accountName, accountlenght, NULL); 

		//���T��
		//���T������
		recv(Connection, (char*)&bufferlength, sizeof(int), NULL);
		char * rcvMessage = new char[bufferlength + 1];
		rcvMessage[bufferlength] = '\0';
		//���T��
		recv(Connection, rcvMessage, bufferlength, NULL);

		//��ܵo�ܪ̱b��
		std::cout << accountName << std::endl;
		//��ܰT��
		std::cout << rcvMessage << std::endl;
		//��ܤ��j�u
		std::cout << dividingLine << std::endl;

		delete[] rcvMessage;
		delete[] accountName;
	}
}



int main()
{
	//��l�� Windows Sockets DLL
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}




	SOCKADDR_IN addr; //client�Ψӳs�u��socket�Ҹj�w����}
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //��} = localhost
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	std::cout << "Please enter Account Name:" << std::endl;
	std::cin >> acc;

	std::cout << "Please enter Account Password:" << std::endl;
	std::cin >> pwd;


	Connection = socket(AF_INET, SOCK_STREAM, NULL); //�إ߳s�u�� socket

	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //���ճs�u
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //�s�u���ѡA�����{��
	}

	//�e�b��
	int acclenght = acc.size(); //�b������
	send(Connection, (char*)&acclenght, sizeof(int), NULL); //�e�X�b������
	send(Connection, acc.c_str(), acclenght, NULL); //�e�X�b��

													//�e�K�X
	int pwdlenght = pwd.size(); //�K�X����
	send(Connection, (char*)&pwdlenght, sizeof(int), NULL); //�e�X�K�X����
	send(Connection, pwd.c_str(), pwdlenght, NULL); //�e�X�K�X
	std::cout << "���ճs�W���A��..." << std::endl; //�s�u���\

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //�إߨϥΪ̪�thread
	std::string sendMessage;//�s��n�e�X�s�u���T����buffer
	while (true)
	{
		std::getline(std::cin, sendMessage); //���o�n�p�T���H
		int bufferlength = sendMessage.size(); 
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //�e�X����
		send(Connection, sendMessage.c_str(), bufferlength, NULL); //�e�X�p�T�W�r
		std::cout << "--------" << std::endl; //���j�u
		Sleep(10);

		std::getline(std::cin, sendMessage); //���o�n�e�X���T��
		bufferlength = sendMessage.size(); //���o�T������
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //�e�X�T������
		send(Connection, sendMessage.c_str(), bufferlength, NULL); //�e�X�T��
		std::cout << "--------" << std::endl; //���j�u
		Sleep(10);
	}
}
