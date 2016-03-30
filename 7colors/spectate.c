#include <stdio.h>  /* printf */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "board.h"
#include "display.h"
#include "auxiliary.h"
#include "strategies.h"

#define BUFFER_SIZE 1000

int main(){


	SDL_Init(SDL_INIT_EVERYTHING);

	struct sockaddr_in addr; // The necessary structure to store all pieces of information about the server (IP address, port, etc)
	int sock; // The socket to connect to the server
	socklen_t addr_size = sizeof(struct sockaddr_in);
	char buffer[BUFFER_SIZE]; // The buffer where we store what we get from the server
	char IP_string[64];

	printf("Please enter the IP address you want to connect to.\n");
	do{
		fgets(IP_string,16,stdin);
	}while(inet_addr(IP_string) <= 0);

	// Creating the necessary structure to store the address you're trying to connect to
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777);
	addr.sin_addr.s_addr = inet_addr(IP_string);
	inet_pton(AF_INET, inet_ntoa(addr.sin_addr), &addr.sin_addr);




	// Creating the client socket
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock == -1){
		printf("Error : couldn't create the socket !");
		return 0;
	}
	// Connexion test
	if(connect(sock,(struct sockaddr *)&addr,addr_size) == -1){
		printf("Error : couldn't connect to the server socket !\n");
		return 0;
	}

	printf("Successfully connection to the server.\n");
	recv(sock,buffer,BUFFER_SIZE,0);
	get_saved_board(buffer);
	init_window();

	while(nextturn()){
		update_board();
		printf("Player 1 : %d\n",score(PLAYER1));
		printf("Player 2 : %d\n",score(PLAYER2));
		printf("\n");

		recv(sock,buffer,BUFFER_SIZE,0);
		play(buffer[0],TEMP,buffer[1]); // The move is done here.
	}

	update_board();
	printf("Player 1 : %d\n",score(PLAYER1));
	printf("Player 2 : %d\n",score(PLAYER2));
	printf("\n");
	printf("Press a key, then Enter to quit");
	getchar();


	(void)shutdown(sock, SHUT_RDWR);
	close(sock);
	SDL_Quit();
	return 0;
}
