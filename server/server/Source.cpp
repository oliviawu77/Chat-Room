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
	void setConnectionNumberCorrespondToAccount(int connectionNumber, int AccountNumber); //�]�wconnection���s���ҹ����쪺account�s��
	bool findAccountAndPassword(std::string acc, std::string pwd,int connectionIndex); //���ұb���M�K�X�A�諸�ܴN�^��1
	bool accountIsLive(int index); //�^�Ǹӱb��O�_�b�u�W�A0:���b�u�W�A1:�b�u�W
	int findConnectNumber(std::string name);
	
private:
	std::string account[10];
	std::string password[10];
	bool accountIsConnected[10]; //��0��ܸ�account�ثe���b�u�W ��1��ܸ�account�b�u�W
	int connectionNumberCorrespondToAccount[10]; //�����C��connection�����쪺���P�b���A�̭��s���O�L�baccount�o�Ӱ}�C�̪��s��
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
				correct = true; //��ܦ����b���A�K�X�]�O�諸�A�ҥH�]��true�H��i�H�������X�j��
				setConnectionNumberCorrespondToAccount(connectionIndex, i);
				setAccountConnect(i); //�N�ӱb��]���W�u
				break;
			}
			else break; //��ܦ����b���A���O�K�X�O�����A�ҥH�������X
		}
	}
	return correct;
}

void Account::setConnectionNumberCorrespondToAccount(int connectionNumber, int accountIndex)
{
	connectionNumberCorrespondToAccount[connectionNumber] = accountIndex;
}

std::string Account::getAccountName(int connectionNumber) //�z�Lconnection index�ܼƪ��Ʀr�h������쪺�b���W�r�æ^�Ǧr��
{
	int number = connectionNumberCorrespondToAccount[connectionNumber];
	return account[number];
}

std::string Account::getAccountNameByAccountIndex(int index)//�^�Ǹӱb��s�����b��r��
{
	return account[index];
}

void Account::setAccountConnect(int accountIndex)
{
	accountIsConnected[accountIndex] = 1;
}

int Account::findConnectNumber(std::string name) //�ΦW�r���ӱb�����s���A�b���ӽs���b���ݩ����connectio number
{
	for (int i = 0; i < 10; i++)
	{
		if (name == account[i]) //���b�����s���F
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
	return -1;//�S���
}

Account ac;


void ClientHandlerThread(int index) //index = the index in the SOCKET Connections array
{
	ConnectionCounter++;

	int acclen;
	int pwdlen;

	//���b��
	recv(Connections[index], (char*)&acclen, sizeof(int), NULL);
	char * UserAccount = new char[acclen + 1];
	UserAccount[acclen] = '\0';
	recv(Connections[index], UserAccount, acclen, NULL);

	//���K�X
	recv(Connections[index], (char*)&pwdlen, sizeof(int), NULL);
	char * UserPassWord = new char[pwdlen + 1];
	UserPassWord[pwdlen] = '\0';
	recv(Connections[index], UserPassWord, pwdlen, NULL);

	//����
	if (!ac.findAccountAndPassword(UserAccount, UserPassWord, index))
	{
		std::cout << "�b��s�u����(�b���αK�X���~)" << std::endl;
		while (1);//�ҥ~�B�z
	}
	
	std::string accountName = ac.getAccountName(index);//�{�b�s�W�u���o�ӱb��W��

	int bufferlength; //buffer����
	int accountlenght = accountName.size(); //�b�����
	int dividingLineLen;

	//�e�w��T��
	std::string message = "Hi!"; 
	int WelcomeMsgght = message.size(); 
	//�ǰe�w��T��������
	send(Connections[index], (char*)&WelcomeMsgght, sizeof(int), NULL);
	//�ǰe�w��T��
	send(Connections[index], message.c_str(), WelcomeMsgght, NULL);
	//�e�b������
	send(Connections[index], (char*)&accountlenght, sizeof(int), NULL);//�b���W�٪���
	//�e�b���W��
	send(Connections[index], accountName.c_str(), accountlenght, NULL);//�b���W��

	//�e���j�u(�u�n�e�@���N�n�]�����@��)
	std::string dividingLine = "========================="; //���j�u�T��
	dividingLineLen = dividingLine.size();
	//�e���j�u����
	send(Connections[index], (char*)&dividingLineLen, sizeof(int), NULL);
	//�ǰe���j�u�r��
	send(Connections[index], dividingLine.c_str(), dividingLineLen, NULL);




	//�e�X"userlist"�r��	
	std::string userlist = "userlist";
	int userlistLen = userlist.size();
	//�ǰe"userlist"����
	send(Connections[index], (char*)&userlistLen, sizeof(int), NULL);
	//�ǰe"userlist"�r��	
	send(Connections[index], userlist.c_str(), userlistLen, NULL);


	//�e�Xuser list
	int z;
	std::string liveAccoumt;
	for (z = 0; z < 10; z++)
	{
		if (ac.accountIsLive(z)) //��ܸӱb��s���b�u�W
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
	
	std::string accName = accountName; //�S�[�_�����b���r��
	accountName = accountName + ":";
	accountlenght = accountlenght + 1;

	//�Ĥ@���e�X�T���ɷ|���W�䧮�h�e�ťհT���A�ҥH�b�o�̥������Ӧh���ťհT���åB���e���O�H
	recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL);
	char * buffer = new char[bufferlength];
	recv(Connections[index], buffer, bufferlength, NULL);
	
	while (1)
	{
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL); //���ϥΪ̷Q�n�p�T���H
		char * talkTo = new char[bufferlength+1];
		talkTo[bufferlength] = '\0';
		recv(Connections[index], talkTo, bufferlength, NULL);

		int talkToTheIndex = ac.findConnectNumber(talkTo); //�o��n�ǰT����H���W�r�F�A�M��n�h���ӦW�r��connection index
		
		recv(Connections[index], (char*)&bufferlength, sizeof(int), NULL); //���ϥΪ̶ǹL�Ӫ��T��
		char * buffer = new char[bufferlength+1];
		buffer[bufferlength] = '\0';
		recv(Connections[index], buffer, bufferlength, NULL);


		send(Connections[talkToTheIndex], (char*)&accountlenght, sizeof(int), NULL);//�e�X�o�ܪ̱b���W�٪���
		send(Connections[talkToTheIndex], accountName.c_str(), accountlenght, NULL);//�e�X�o�ܪ̱b���W��

		send(Connections[talkToTheIndex], (char*)&bufferlength, sizeof(int), NULL);//�e�X�T������
		send(Connections[talkToTheIndex], buffer, bufferlength, NULL);//�e�X�T��

		
		delete[] buffer;

		if (talkToTheIndex == -1) //��ܦ��ϥΪ̤U�u�A�N������server���o��thread�����A��
		{
			std::string fail = "fail to connect";
			std::cout << fail << std::endl;
			break;
		}
		
	}
}

int main()
{
	//Winsock ��l�]�m
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //�N��S�����\�]�m
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	SOCKADDR_IN addr; //�j�w ��ť��socket�� ��m
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //�s���d��b���x�D���W
	addr.sin_port = htons(1111); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //�إߺ�ť�s�u��socket
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //�j�w�o��socket����}
	listen(sListen, SOMAXCONN); //�}�l��ť SOMAXCONN:�̤j�s�uqueue����

	SOCKET newConnection; //�����s�s�u��socket
	for (int i = 0; i < 10; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //�����s�s�u
		if (newConnection == 0) //client�s�u����
		{
			std::cout << "Failed to accept the client's connection." << std::endl;
		}
		else //client�s�u���\
		{
			Connections[i] = newConnection;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL); //�إߤ@�ӥh�B�z��client��thread
		}
	}

	system("pause");
	return 0;
}