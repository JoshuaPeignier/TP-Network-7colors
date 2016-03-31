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

#define BUFFER_SIZE 1000

// Initialising everything necessary for online interactions
socklen_t addr_size;
char buffer[BUFFER_SIZE];
int allow_spectate = 0;
int yes = 1;
char current_color;
//Socket and address dedicated to the spectator
int spectatorSock; // The socket we will open
int spectatorReadSock; // The socket where we will store the one returned by accept()
struct sockaddr_in spectatorAddr; // The variable which contains each piece of information about the current client (IP address, port, etc)
//Same here for a player who plays online
//int sock; // The socket we will open
//int readSock; // The socket where we will store the one returned by accept()
//struct sockaddr_in addr; // The variable which contains each piece of information about the current client (IP address, port, etc)

// Bind-Listen-Accept
void bla_spectators(){
	// Creating the necessary structure to store the address you want to connect to
	addr_size = sizeof(struct sockaddr_in);
	memset(&spectatorAddr, 0, sizeof spectatorAddr); //memorizes a constant (thanks wikipedia)
	spectatorAddr.sin_family = AF_INET;
	spectatorAddr.sin_port = htons(7777); // the port that will be used one the connection is etablished (hopefully wll bring luck)
	spectatorAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Creating the socket
	spectatorSock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); //!!! AF est une generalisation de PF (essayons de remplacer)
	if(spectatorSock == -1){
		perror("couldn't create the socket !\n");
		exit(EXIT_FAILURE);
	}

	// Avoiding the "address already in use" error
	if(setsockopt(spectatorSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("couldn't avoid the 'address already in use' error");
		exit(EXIT_FAILURE);
	}

	// Binding the socket to previous address
	if( bind(spectatorSock, (struct sockaddr*) &spectatorAddr, addr_size) == -1){
		perror("couldn't bind the socket and the adress !\n");
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}

	// Listening (finding and listing plugs)
	if(listen(spectatorSock, 1024) == -1){
		perror("sock is deaf and cannot listen !");
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}

	// Accepting the next request
	spectatorReadSock = accept(spectatorSock, (struct sockaddr *) &spectatorAddr, &addr_size);
	if(spectatorReadSock == -1){
		perror("couldn't accept request.\n");
		close(spectatorReadSock);
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}
}


//functions will be used in 7color.c to decide whever to wait for a spectator or not
void allow_spectators(){allow_spectate = 1;}
void ban_spectators(){allow_spectate = 2;}
int spectate(){return allow_spectate;}

void send_board(){ //at the bigginig must send the initial bord
	copy_board(buffer);
	buffer[BOARD_SIZE*BOARD_SIZE] = '\0';

	if(send(spectatorReadSock, buffer, BUFFER_SIZE, 0) == -1){
		perror("couldn't send anything");
		close(spectatorReadSock);
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}
}

//used after each game turn, with it the observer can update his game.
void send_move(char player){
	buffer[0] = player;
	buffer[1] = current_color;
	buffer[2] = '\0';
	if(send(spectatorReadSock, buffer, BUFFER_SIZE, 0) == -1){
		perror("couldn't send anything");
		close(spectatorReadSock);
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}
}

void spectators_quit(){
	shutdown(spectatorSock, SHUT_RDWR);
	shutdown(spectatorReadSock, SHUT_RDWR);
	close(spectatorReadSock);
	close(spectatorSock);
}
