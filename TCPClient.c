#include "TCPClient.h"

#include <stdio.h>
#include <WinSock2.h>
#include <string.h>

#pragma comment(lib,"ws2_32")

int createClient(struct _CLIENT **clients, short *clientCount, char *uuid, const char *ip, int port) {
	SOCKADDR_IN target;
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (err != 0) {
		printf("WSAStartup error %ld", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	target.sin_family = AF_INET; // address family Internet
	target.sin_port = htons(port); //Port to connect on
	target.sin_addr.s_addr = inet_addr(ip); //Target IP

	SOCKET *s = (SOCKET *)malloc(sizeof(SOCKET));
	*s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (*s == INVALID_SOCKET)
	{
		WSACleanup();
		return -1; //Couldn't create the socket
	}

	if (SOCKET_ERROR == connect(*s, (struct sockaddr *)&target, sizeof(target))) {
		printf("ERROR : Failed socket conenction.\n");
		return -1;
	}

	insertClient(clients, clientCount, uuid, s, target);

	return 1;
}
void insertClient(struct _CLIENT **clients, short *clientCount, char *uuid, SOCKET *s, SOCKADDR_IN saddr) {
	*clientCount += 1;

	//초기화 및 대입
	struct _CLIENT *newClient = (CLIENT *)malloc(sizeof(CLIENT));
	memset(newClient, 0, sizeof(CLIENT));

	newClient->uuid = (char *)malloc(strlen(uuid));
	strcpy(newClient->uuid, uuid);
	newClient->s = s;
	newClient->saddr = saddr;
	newClient->next = NULL;

	//리스트에 삽입
	if (*clients == NULL) {
		*clients = newClient;
	}
	else {
		struct _CLIENT* current = *clients;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newClient;
	}
}
int sendCommand(struct _CLIENT *clients, int index, const char* command) {
	struct _CLIENT *current = clients;

	if (index < 1) return -1;

	while (index != 1) {
		current = current->next;
		index--;
	}

	if (send(*(current->s), command, strlen(command), 0) == SOCKET_ERROR) {
		return -1;
	}

	return 1;
}

int removeClient(struct _CLIENT **clients, short *clientCount, int index) {
	if (index > *clientCount || index <= 0) return -1;

	struct _CLIENT* removeNode = NULL;

	if (index == 1) {
		//첫 번째 삭제
		removeNode = *(clients);
		*(clients) = (*clients)->next;
	}
	else {
		struct _CLIENT *current = *clients;
		for (; index != 2; index--) {
			current = current->next;
		}
		removeNode = current->next;
		current->next = current->next->next;
	}

	closesocket(*(removeNode->s)); //소켓 연결 끊기
	free(removeNode); //동적할당 해제

	*clientCount -= 1;

	return 1;
}

void removeAllClient(struct _CLIENT **clients) {
	struct _CLIENT* removeNode = NULL;
	while (*clients != NULL) {
		removeNode = *clients;

		*clients = (*clients)->next;

		closesocket(*(removeNode->s));
		free(removeNode);
	}
}

void printClients(struct _CLIENT *clients) {
	struct _CLIENT *current = clients;
	int index = 0, localPort = 0;

	struct sockaddr_in sin;

	printf("\t==============================\n");
	while (current != NULL) {
		if (getsockname(*(current->s), (struct sockaddr*)&sin, sizeof(sin) == -1)) {
			localPort = -1;
		}
		else {
			localPort = ntohs(sin.sin_port);
		}
		printf("\t  [%2d] %s : %d\n", ++index, current->uuid, localPort);
		current = current->next;
	}
	printf("\t==============================\n");
}