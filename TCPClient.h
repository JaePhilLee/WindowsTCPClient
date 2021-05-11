#pragma once

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
	#include <WinSock2.h>
	
	#define _WINSOCK_DEPRECATED_NO_WARNINGS

	#define BUF_SIZE 256

	typedef struct _CLIENT {
		char *uuid;
		SOCKET *s;
		SOCKADDR_IN saddr;

		struct _CLIENT *next;
	}CLIENT;

	//家南 包访
	int createClient(struct _CLIENT **clients, short *clientCount, char *uuid, const char *ip, int port);
	int removeClient(struct _CLIENT **clients, short *clientCount, int index);
	void removeAllClient(struct _CLIENT **clients);
	int sendCommand(struct _CLIENT *clients, int index, const char* command);

	//府胶飘 包访
	void insertClient(struct _CLIENT **clients, short *clientCount, char *uuid, SOCKET *s, SOCKADDR_IN saddr);
	void printClients(struct _CLIENT *clients);
#endif