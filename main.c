#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#include "TCPClient.h"

#define IP_ADDRESS "127.0.0.1"
#define PORT 23800

void run();
void runOption(int);
int showOptions();
char *getCommandOptions();

CLIENT *clients = NULL;
short clientCount = 0;

int main()
{
	run();

	return 0;
}

void run() {
	int op = 0;

	while (1) {
		op = showOptions();

		runOption(op);

		if (op == 0) break;
	}
}

void runOption(int op) {
	system("cls");

	switch (op) {
	case 0: //종료
		removeAllClient(&clients);
		break;
	case 1: //목록 보기
		printClients(clients);
		break;
	case 2: //명령 전송
	{
		int index = 0;

		printClients(clients);
		printf("# Index : ");
		scanf("%d", &index);

		char *command = getCommandOptions();
		if (command == NULL) {
			printf("[커맨드 오류]\n");
			break;
		}

		if (sendCommand(clients, index, command) == -1) {
			printf("[전송 오류]\n");
		}
		else {
			printf("[전송 완료]\n");
		}
	}
		break;
	case 3: //클라이언트 생성
	{
		char uuid[BUF_SIZE] = { 0, };

		printf("# UUID : ");
		scanf("%s", uuid);

		createClient(&clients, &clientCount, uuid, IP_ADDRESS, PORT);
	}
	break;
	case 4: //클라이언트 삭제
	{
		int index = 0;

		printClients(clients);

		printf("\n\n# Index : ");
		scanf("%d", &index);

		removeClient(&clients, &clientCount, index);

		printf("\n[%02d] 삭제 완료", index);
	}
	break;
	default: //Unknown
		printf("ERROR : Unknown Command\n\n");
		break;
	}
}
int showOptions() {
	int op = 0;

	printf("\n\n##############################\n");
	printf("[1] 목록 보기 (Count : %d)\n", clientCount);
	printf("[2] 명령 전송\n");
	printf("[3] 클라이언트 생성\n");
	printf("[4] 클라이언트 제거\n");
	printf("[0] 나가기\n");
	printf("##############################\n");
	printf(" => ");
	scanf("%d", &op);

	return op;
}
char *getCommandOptions() {
	int commandLen = 5, option = 0;
	char buff[BUF_SIZE] = { 0, };
	char *commands[] = {
		"111111111111",
		"222222222222222",
		"33333333333333333",
		"444444444444444444",
		"5555555555555555555"
	};

	printf("\n\n######### Commands #########\n");
	for (int i = 0; i < commandLen; i++) {
		printf("# [%d] %s\n", i + 1, commands[i]);
	}
	printf("# [%d] 직접 입력\n", commandLen + 1);
	printf("############################\n");
	printf(" => ");
	scanf("%d", &option);

	if (option < 1 || option > commandLen + 1) 
		return NULL;

	if (option == commandLen + 1) {
		printf("# 입력 : ");
		scanf("%s", buff);

		return buff;
	}

	return commands[option - 1];
}