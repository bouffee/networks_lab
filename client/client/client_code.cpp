#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#pragma warning(disable:4996) 
#pragma comment (lib, "Ws2_32.lib")
#define SERVER_PORT 3817

using namespace std;

int main() {
	SOCKET ClientSock = INVALID_SOCKET;
	CHAR DefaultIPAddr[] = "127.0.0.1";
	struct sockaddr_in Addr;

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult < 0) {
		cout << endl << "error" << endl;
		getchar();
		exit(0);
	}

	ClientSock = socket(AF_INET, SOCK_STREAM, 0);
	//set server adress
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	Addr.sin_port = htons(SERVER_PORT);

	char hostname[] = "                ";
	DWORD size = 20;

	//connecting to the server
	int u;
	u = connect(ClientSock, (struct sockaddr*)&Addr, sizeof(Addr));
	if (u == INVALID_SOCKET) {
		cout << "Connection failed";
		getchar();
		return 0;
	}
	else cout << "Connection is successful\n";

	int bytes = 0;
	char rez[34] = "";
	char buf[20] = "beginning";
	int len;
	char s_word[5] = "stop";
	int size_code = sizeof(s_word);
	int check = 0;

	while (1) {
		cout << "input: ";
		cin >> buf;
		len = sizeof(buf);
		send(ClientSock, (char*)buf, len, 0);
		bytes = recv(ClientSock, (char*)rez, sizeof(rez), 0);
		if (rez[0] == 's' && rez[1] == 't' && rez[2] == 'o' && rez[3] == 'p') {
			break;
		}
		if (bytes == -1) cout << "result with a mistake";
		else cout << rez << endl;
	}

	shutdown(ClientSock, 2);
	closesocket(ClientSock);
	WSACleanup();
	cout << endl << "Exit from the client part of programme" << endl;
	getchar();
	return 0;
}