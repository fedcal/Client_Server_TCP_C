/*
 ============================================================================
 Name        : Server1.c
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

int main(int argc, char *argv[]) {
	//numero di porta
	int port;
	if(argc>1){
		port=atoi(argv[1]);
	}else
		port=PROTOPORT;
	printf("Porta: %d",port);
	if(port<0){
		printf("numero di prota non valido\n",argv[1]);
		//return 0;
	}

	#if defined WIN32
		WSADATA wsa_data;
		int result=WSAStartup(MAKEWORD(2,2),&wsa_data);
		if(result!=0){
			printf("Error At WSAStartup\n");
			//return 0;
		}else{
			printf("\n Sei in un sistema windows WSADATA ricevuta\n");
		}
	#endif

	int socket_server;
	socket_server=socket(AF_INET,SOCK_STREAM,0);
	if(socket_server==INVALID_SOCKET){
		printf("socket server non creata");
		//clearwinsock();
		//return -1;
	}else{
		printf("Server_Socket creata %d\n",socket_server);
	}

	struct sockaddr_in sad;
	memset(&sad,0,sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = INADDR_ANY;
	sad.sin_port = htons(port);

	//bind
	int bind_socket=bind(socket_server, (struct sockaddr*) &sad, sizeof(sad));
	if(bind_socket<0){
		printf("bind non eseguita\n");
		closesocket(socket_server);
		clearwinsock();
		return -1;
	}else{
		printf("bind eeguita\n");
	}
	int listen_size=listen(socket_server,QLEN);
	if(listen_size<0){
		printf("listen non eseguita\n");
		//closesocket(socket_server);
		//clearwinsock();
		//return -1;
	}else{
		printf("listen eseguita\n");
	}
	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int client_socket; // socket descriptor for the client
	int client_len; // the size of the client address
	printf("Waiting for a client to connect...");
	fflush(stdout);
	client_len=sizeof(cad);
	while(1){
		client_socket=accept(socket_server, (struct sockadr*)&cad, &client_len);
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		//printf("client_len: %d",client_len);

		/*if(client_socket<0){
			printf("accept() failed.\n");
			// CHIUSURA DELLA CONNESSIONE
			//closesocket(my_socket);
			clearwinsock();
			return 0;
		}*/

		struct msgStruct msg;
		strcpy(msg.a,"Connessione avvenuta\n");
		strcpy(msg.b,"");

		send(client_socket, &msg, sizeof(msg),0);

		recv(client_socket,&msg,sizeof(msg),0);
		printf("Stringa 1 ricevuta: %s \n", msg.a);
		printf("Stringa 2 ricevuta: %s \n", msg.b);
		fflush(stdout);
	    for(int i = 0; msg.a[i]!='\0';i++)
		{
			if (isalpha(msg.a[i]))
				msg.a[i] = toupper(msg.a[i]);
		}
	    for(int i = 0; msg.b[i]!='\0';i++)
		{
			if (isalpha(msg.b[i]))
				msg.b[i] = tolower(msg.b[i]);
		}
	    send(client_socket, &msg, sizeof(msg),0);

	}
	closesocket(socket_server);
	clearwinsock();

	//return EXIT_SUCCESS;
}
