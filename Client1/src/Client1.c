/*
 ============================================================================
 Name        : Client1.c
 Author      : Federico Calò
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>
#else
#define closessocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "protocol.h"

void clearwinsock() {
	#if defined WIN32
		WSACleanup();
	#endif
}

int main(void) {
	#if defined WIN32
		WSADATA wsa_data;
		int result = WSAStartup(MAKEWORD(2 ,2), &wsa_data);
		if (result != 0) {
			printf ("error at WSASturtup\n");
			return -1;
		}
	#endif
	// CREAZIONE DELLA SOCKET
	int c_socket;
	//c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	c_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (c_socket == INVALID_SOCKET) {
		printf("socket creation failed.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(27016); // Server port

	//CONNESSIONE AL SERVER
	int connection=connect(c_socket, (struct sockaddr *)&sad, sizeof(sad));
	if(connection<0){
		printf("connessione fallita\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	struct msgStruct msg;
	recv(c_socket,&msg,sizeof(msg),0);
	printf("%s",msg.a);
	int uscire=1;
	do{
		char stringa1[512];
		char stringa2[512];
		fflush(stdout);
		printf("Inserisci stringa 1: \n");
		fflush(stdin);
		scanf("%s",&stringa1);
		printf("Inserisci stringa 2: \n");
		fflush(stdin);
		scanf("%s",&stringa2);
		strcpy(msg.a,stringa1);
		strcpy(msg.b,stringa2);

		printf("\n Striga 1 inserita: %s", msg.a);
		printf("\n Striga 2 inserita: %s", msg.b);

		int invio=send(c_socket, &msg, sizeof(msg),0);
		if(invio<0){
			printf("\n errore invio\n");
		}
		fflush(stdout);
		recv(c_socket,&msg,sizeof(msg),0);
		printf("\n Striga 1 convertita: %s", msg.a);
		printf("\n Striga 2 convertita: %s", msg.b);
		printf("\n uscire?\n");
		scanf("%d", uscire);
	}while(uscire==1);

	return EXIT_SUCCESS;
}
