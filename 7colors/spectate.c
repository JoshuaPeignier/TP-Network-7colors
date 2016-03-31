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
	memset(&addr, 0, sizeof addr); //creates a memory space in which to store the constant for address
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777); // the port dedicated to observers
	addr.sin_addr.s_addr = inet_addr(IP_string);
	inet_pton(AF_INET, inet_ntoa(addr.sin_addr), &addr.sin_addr);




	// Creating the client socket
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP); // AF_INET is the protocol domain, SOCK_STREAM indicates we want a two way connection and IPROTO8TCP is the protocol
	if(sock == -1){
		printf("Error : couldn't create the socket !");
		return 0;
	}
	// Connexion to the server
	if(connect(sock,(struct sockaddr *)&addr,addr_size) == -1){
		printf("Error : couldn't connect to the server socket !\n"); //error
		return 0;
	}

	printf("Successfully connection to the server.\n");
	recv(sock,buffer,BUFFER_SIZE,0); //get grid
	get_saved_board(buffer); //save the initial grid
	init_window(); //open window

	while(nextturn()){
		update_board(); //display bord
		printf("Player 1 : %d\n",score(PLAYER1));
		printf("Player 2 : %d\n",score(PLAYER2));
		printf("\n");

		if(recv(sock,buffer,BUFFER_SIZE,0) == -1){ //receive informatition, (player, color) for the turn
			perror("couldn't receive anything");
			close(sock);
			exit(EXIT_FAILURE);
		}
		play(buffer[0],TEMP,buffer[1]); // The move is done here the game is reconstructed.
	}
	//end of game update and score
	update_board();
	printf("Player 1 : %d\n",score(PLAYER1));
	printf("Player 2 : %d\n",score(PLAYER2));
	printf("\n");
	printf("Press a key, then Enter to quit");
	getchar();

	//leaving the game
	shutdown(sock, SHUT_RDWR);
	close(sock);
	SDL_Quit();
	return 0;
}
