#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

#include <WinSock2.h>
#include <iostream>
#include <string>
#include <thread>
#define _WINSOCK_DEPRECATED_NO_WARNING


SOCKET Connections[100];
int ConnectionCounter = 0;

class Account
{
public:
	Account()
	{
		for (int i = 0; i < 10; i++)
		{
			accountIsConnected[i] = 0;
		}

		account[0] = "AAA";
		password[0] = "1234";

		account[1] = "BBB";
		password[1] = "1234";

		account[2] = "CCC";
		password[2] = "1234";

		account[3] = "DDD";
		password[3] = "1234";

		account[4] = "EEE";
		password[4] = "1234";

		account[5] = "FFF";
		password[5] = "1234";

		account[6] = "GGG";
		password[6] = "1234";

		account[7] = "HHH";
		password[7] = "1234";

		account[8] = "III";
		password[8] = "1234";

		account[9] = "JJJ";
		password[9] = "1234";
	}
	std::string getAccountName(int index);
	std::string getAccountNameByAccountIndex(int index);
	void setAccountConnect(int index);
	void setConnectionNumberCorrespondToAccount(int connectionNumber, int AccountNumber); //設定connection的編號所對應到的account編號
	bool findAccountAndPassword(std::string acc, std::string pwd,int connectionIndex); //驗證帳號和密碼，對的話就回傳1
	bool accountIsLive(int index); //回傳該帳戶是否在線上，0:不在線上，1:在線上
	int findConnectNumber(std::string name);
	
private:
	std::string account[10];
	std::string password[10];
	bool accountIsConnected[10]; //為0表示該account目前不在線上 為1表示該account在線上
	int connectionNumberCorrespondToAccount[10]; //紀錄每個connection對應到的不同帳號，裡面存的是他在account這個陣列裡的編號
};


bool Account::accountIsLive(int accountIndex)
{
	return accountIsConnected[accountIndex];
}
bool Account::findAccountAndPassword(std::string acc, std::string pwd,int connectionIndex)
{
	bool correct = false;
	int i;
	for (i = 0; i < 10; i++)
	{
		if (acc == account[i])
		{
			if (pwd == password[i])
			{
				correct = true; //表示有找到帳號，密碼也是對的，所以設為true以後可以直接跳出迴圈
				setConnectionNumberCorrespondToAccount(connectionIndex, i);
				setAccountConnect(i); //將該帳戶設為上線
				break;
			}
			else break; //表示有找到帳號，但是密碼是錯的，所以直接跳出
		}
	}
	return correct;
}

void Account::setConnectionNumberCorrespondToAccount(int connectionNumber, int accountIndex)
{
	connectionNumberCorrespondToAccount[connectionNumber] = accountIndex;
}

std::string Account::getAccountName(int connectionNumber) //透過connection index變數的數字去找對應到的帳號名字並回傳字串
{
	int number = connectionNumberCorrespondToAccount[connectionNumber];
	return account[number];
}

std::string Account::getAccountNameByAccountIndex(int index)//回傳該帳戶編號的帳戶字串
{
	return account[index];
}

void Account::setAccountConnect(int accountIndex)
{
	accountIsConnected[accountIndex] = 1;
}

int Account::findConnectNumber(std::string name) //用名字找到該帳號的編號，在找到該編號在隸屬於哪個connectio number
{
	for (int i = 0; i < 10; i++)
	{
		if (name == account[i]) //找到帳號的編號了
		{
			for(int k = 0; k < 10; k++)
			{
				if (connectionNumberCorrespondToAccount[k] == i)
				{
					return k;
				}
			}
		}

	}
	return -1;//沒找到
}

Account ac;


void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	ConnectionCounter++;

	int acclen;
	int pwdlen;

	//收帳號
	recv(Connections[index], (char*)&acclen, sizeof(int), NULL);
	char * UserAccount = new char[acclen + 1];
	UserAccount[acclen] = '\0';
	recv(Connections[index], UserAccount, acclen, NULL);

	//收密碼
	recv(Connections[index], (char*)&pwdlen, sizeof(int), NULL);
	char * UserPassWord = new char[pwdlen + 1];
	UserPassWord[pwdlen] = '\0';
	recv(Connections[index], UserPassWord, pwdlen, NULL);

	//驗證
	if (!ac.findAccountAndPassword(UserAccount, UserPassWord, index))
	{
		std::cout << "帳戶連線失敗(帳號或密碼錯誤)" << std::endl;
		while (1);//例外處理
	}
	
	std::string accountName = ac.getAccountName(index);//現在連上線的這個帳戶名稱

	int bufferlength; //buffer長度
	int accountlenght = accountName.size(); //帳戶長度
	int dividingLineLen;

	//送歡迎訊息
	std::string message = "Hi!"; 
	int WelcomeMsgght = message.size(); 
	//傳送歡迎訊息的長度
	send(Connections[index], (char*)&WelcomeMsgght, sizeof(int), NULL);
	//傳送歡迎訊息
	send(Connections[index], message.c_str(), WelcomeMsgght, NULL);
	//送帳號長度
	send(Connections[index], (char*)&accountlenght, sizeof(int), NULL);//帳號名稱長度
	//送帳號名稱
	send(Connections[index], accountName.c_str(), accountlenght, NULL);//帳號名稱

	//送分隔線(只要送一次就好因為都一樣)
	std::string dividingLine = "========================="; //分隔線訊息
	dividingLineLen = dividingLine.size();
	//送分隔線長度
	send(Connections[index], (char*)&dividingLineLen, sizeof(int), NULL);
	//傳送分隔線字串
	send(Connections[index], dividingLine.c_str(), dividingLineLen, NULL);




	//送出"userlist"字串	
	std::string userlist = "userlist";
	int userlistLen = userlist.size();
	//傳送"userlist"長度
	send(Connections[index], (char*)&userlistLen, sizeof(int), NULL);
	//傳送"userlist"字串	
	send(Connections[index], userlist.c_str(), userlistLen, NULL);


	//送出user list
	int z;
	std::string liveAccoumt;
	for (z = 0; z < 10; z++)
	{
		if (ac.accountIsLive(z)) //表示該帳戶編號在線上
		{
			liveAccoumt = ac.getAccountNameByAccountIndex(z);
			acclen = liveAccoumt.size();
			send(Connections[index], (char*)&acclen, sizeof(int), NULL);
			send(Connections[index], liveAccoumt.c_str(), acclen, NULL);
		}
		else 
		{
			liveAccoumt = "(X)"+ ac.getAccountNameByAccountIndex(z);
			acclen = liveAccoumt.size()+1;
			send(Connections[index], (char*)&acclen, sizeof(int), NULL);
			send(Connections[index], liveAccoumt.c_str(), acclen, NULL);
		}
	}
	
	std::string accName = accountName; //沒加冒號的帳號字串
	accountName = accountName + ":";
	accountlenght = accountlenght + 1;

	//第一次送出訊息時會莫名其妙多送空白訊息，所以在這裡先收那個多的空白訊息並且不送給別人
	recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL);
	char * buffer = new char[bufferlength];
	recv(Connections[index], buffer, bufferlength, NULL);
	
	while (1)
	{
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL); //收使用者想要私訊的人
		char * talkTo = new char[bufferlength+1];
		talkTo[bufferlength] = '\0';
		recv(Connections[index], talkTo, bufferlength, NULL);

		int talkToTheIndex = ac.findConnectNumber(talkTo); //得到要傳訊的對象的名字了，然後要去找到該名字的connection index
		
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL); //收使用者傳過來的訊息
		char * buffer = new char[bufferlength+1];
		buffer[bufferlength] = '\0';
		recv(Connections[index], buffer, bufferlength, NULL);


		send(Connections[talkToTheIndex], (char*)&accountlenght, sizeof(int), NULL);//送出發話者帳號名稱長度
		send(Connections[talkToTheIndex], accountName.c_str(), accountlenght, NULL);//送出發話者帳號名稱

		send(Connections[talkToTheIndex], (char*)&bufferlength, sizeof(int), NULL);//送出訊息長度
		send(Connections[talkToTheIndex], buffer, bufferlength, NULL);//送出訊息

		
		delete[] buffer;

		if (talkToTheIndex == -1) //表示有使用者下線，就直接讓server的這個thread結束服務
		{
			std::string fail = "fail to connect";
			std::cout << fail << std::endl;
			break;
		}
		
	}
}

int main()
{
	//Winsock 初始設置
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //代表沒有成功設置
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //綁定 監聽的socket的 位置
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //廣播範圍在本台主機上
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //建立監聽連線的socket
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //綁定這個socket的位址
	listen(sListen, SOMAXCONN); //開始監聽 SOMAXCONN:最大連線queue長度

	SOCKET newConnection; //接收新連線的socket
	for (int i = 0; i < 10; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //接收新連線
		if (newConnection == 0) //client連線失敗
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //client連線成功
		{
			Connections[i] = newConnection;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //建立一個去處理該client的thread
		}
	}

	system("pause");
	return 0;
}