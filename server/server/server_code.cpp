#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma warning(disable:4996) 
#pragma comment (lib, "Ws2_32.lib")
#define SERVER_PORT 3817

using namespace std;

DWORD WINAPI stream(LPVOID lpParam) {
	SOCKET Conn =*(SOCKET*)lpParam;

	int bytes = 0;
	char exit[20] = "exit";
	char buf_in[20];
	char s_word[5] = "stop";
	char buf_out[34] = "Received";
	while (1) {
		bytes = recv(Conn, (char*)buf_in, sizeof(buf_in), 0);
		cout << buf_in << endl;

		if (bytes == SOCKET_ERROR) {
			cout << "Didn't send";
			break;
		}

		if (buf_in[0] == 's' && buf_in[1] == 't' && buf_in[2] == 'o' && buf_in[3] == 'p') {
			send(Conn, (char*)s_word, sizeof(s_word), 0);
			break;
		}
		send(Conn, (char*)buf_out, sizeof(buf_out), 0);
	}
	return 0;
}

int main() {
	struct sockaddr_in SrvAddr; //adress struc for a server
	struct sockaddr_in ConnAddr; //adress struc for a client
	memset(&ConnAddr, 0, sizeof(ConnAddr));
	SOCKET SrvSock, Conn;

	WSADATA wsaData; //work with sockets in windows
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult < 0) {
		cout << "error\n";
		getchar();
		exit(0);
	}

	SrvSock = socket(AF_INET, SOCK_STREAM, 0); //create socket

	//set server adress
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SrvAddr.sin_port = htons(SERVER_PORT);

	bind(SrvSock, (sockaddr*)&SrvAddr, sizeof SrvAddr);
	listen(SrvSock, 5);

	cout << "Server is working...\n";
	int AddrLen = sizeof(ConnAddr);
	DWORD ThreadId;
	HANDLE hThread;

	while (1) {
		Conn = accept(SrvSock, (struct sockaddr*)&ConnAddr, &AddrLen); //waiting for the client
		
		hThread = CreateThread(
			NULL,            
			0,                
			stream,        
			&Conn,  
			0,                
			&ThreadId);        

		if (hThread == NULL)  printf("CreateThread failed.");

	}
	CloseHandle(hThread);

	shutdown(Conn, 2);
	closesocket(Conn);

	_getch();
	return 0;
}