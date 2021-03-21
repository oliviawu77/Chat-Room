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
	int dividingLinelenght; //分隔線長度
	int bufferlength; //訊息長度
	int accountlenght; //發話帳戶名字長度

	//收歡迎訊息
	//收訊息長度
	recv(Connection, (char*)&bufferlength, sizeof(int), NULL);
	char * rcvMessage = new char[bufferlength + 1];
	rcvMessage[bufferlength] = '\0';
	//收訊息
	recv(Connection, rcvMessage, bufferlength, NULL);
	std::cout << rcvMessage << std::endl;//顯示訊息(歡迎訊息)

	//收本帳戶名字
	//收帳戶長度
	recv(Connection, (char*)&accountlenght, sizeof(int), NULL);
	char * accountName = new char[accountlenght + 1];
	accountName[accountlenght] = '\0';
	//收帳號
	recv(Connection, accountName, accountlenght, NULL);
	//顯示目前使用者是誰
	std::cout << accountName << std::endl; 


	//收分隔線
	//收分隔線長度
	recv(Connection, (char*)&dividingLinelenght, sizeof(int), NULL);
	char * dividingLine = new char[dividingLinelenght + 1];
	dividingLine[dividingLinelenght + 1] = '\0';
	//收分隔線字串
	recv(Connection, dividingLine, dividingLinelenght, NULL);
	//顯示分隔線
	std::cout << dividingLine << std::endl;


	//收"userlist"字串
	int userListLenght;
	//收"userlist"長度
	recv(Connection, (char*)&userListLenght, sizeof(int), NULL);
	char * userlist = new char[userListLenght + 1];
	userlist[userListLenght] = '\0';
	//收"userlist"字串
	recv(Connection, (char*)userlist, userListLenght, NULL); 
	std::cout << userlist << std::endl;


	//收userlist
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
		//收發話帳號
		//收發話帳號長度
		recv(Connection, (char*)&accountlenght, sizeof(int), NULL); 
		char * accountName = new char[accountlenght + 1];
		accountName[accountlenght] = '\0';
		//收帳號
		recv(Connection, accountName, accountlenght, NULL); 

		//收訊息
		//收訊息長度
		recv(Connection, (char*)&bufferlength, sizeof(int), NULL);
		char * rcvMessage = new char[bufferlength + 1];
		rcvMessage[bufferlength] = '\0';
		//收訊息
		recv(Connection, rcvMessage, bufferlength, NULL);

		//顯示發話者帳號
		std::cout << accountName << std::endl;
		//顯示訊息
		std::cout << rcvMessage << std::endl;
		//顯示分隔線
		std::cout << dividingLine << std::endl;

		delete[] rcvMessage;
		delete[] accountName;
	}
}



int main()
{
	//初始化 Windows Sockets DLL
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}




	SOCKADDR_IN addr; //client用來連線的socket所綁定的位址
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //位址 = localhost
	addr.sin_port = htons(1111); //Port = 1111
	addr.sin_family = AF_INET; //IPv4 Socket

	std::cout << "Please enter Account Name:" << std::endl;
	std::cin >> acc;

	std::cout << "Please enter Account Password:" << std::endl;
	std::cin >> pwd;


	Connection = socket(AF_INET, SOCK_STREAM, NULL); //建立連線的 socket

	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //嘗試連線
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return 0; //連線失敗，結束程式
	}

	//送帳號
	int acclenght = acc.size(); //帳號長度
	send(Connection, (char*)&acclenght, sizeof(int), NULL); //送出帳號長度
	send(Connection, acc.c_str(), acclenght, NULL); //送出帳號

													//送密碼
	int pwdlenght = pwd.size(); //密碼長度
	send(Connection, (char*)&pwdlenght, sizeof(int), NULL); //送出密碼長度
	send(Connection, pwd.c_str(), pwdlenght, NULL); //送出密碼
	std::cout << "嘗試連上伺服器..." << std::endl; //連線成功

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //建立使用者的thread
	std::string sendMessage;//存放要送出連線的訊息的buffer
	while (true)
	{
		std::getline(std::cin, sendMessage); //取得要私訊的人
		int bufferlength = sendMessage.size(); 
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //送出長度
		send(Connection, sendMessage.c_str(), bufferlength, NULL); //送出私訊名字
		std::cout << "--------" << std::endl; //分隔線
		Sleep(10);

		std::getline(std::cin, sendMessage); //取得要送出的訊息
		bufferlength = sendMessage.size(); //取得訊息長度
		send(Connection, (char*)&bufferlength, sizeof(int), NULL); //送出訊息長度
		send(Connection, sendMessage.c_str(), bufferlength, NULL); //送出訊息
		std::cout << "--------" << std::endl; //分隔線
		Sleep(10);
	}
}
