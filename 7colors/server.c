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
int spectatorSock; // The socket we will open for spectators
int spectatorReadSock; // The socket for spectators where we will store the one returned by accept()
struct sockaddr_in spectatorAddr; // The variable which contains each piece of information about the current spectator (IP address, port, etc)
int playerSock; // The socket we will open for spectators
int playerReadSock; // The socket for spectators where we will store the one returned by accept()
struct sockaddr_in playerAddr; // The variable which contains each piece of information about the current spectator (IP address, port, etc)
socklen_t addr_size;
char buffer[BUFFER_SIZE]; // The buffer, where we store the received data
int allow_spectate = 0;
int yes = 1;

// Bind-Listen-Accept
static void bla(int sock, int readSock, struct sockaddr_in addr){
	// Creating the necessary structure to store the address you want to connect to
	addr_size = sizeof(struct sockaddr_in);
	memset(&spectatorAddr, 0, sizeof addr); //memorizes a constant (thanks wikipedia)
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777); // the port that will be used one the connection is etablished (hopefully wll bring luck)
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Creating the socket
	sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); //!!! AF est une generalisation de PF (essayons de remplacer)
	if(sock == -1){
		perror("couldn't create the socket !\n");
		exit(EXIT_FAILURE);
	}

	// Avoiding the "address already in use" error
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("couldn't avoid the 'address already in use' error");
		exit(EXIT_FAILURE);
	}

	// Binding the socket to previous address
	if( bind(sock, (struct sockaddr*) &addr, addr_size) == -1){
		perror("couldn't bind the socket and the adress !\n");
		close(spectatorSock);
		exit(EXIT_FAILURE);
	}

	// Listening (finding and listing plugs)
	if(listen(sock, 1024) == -1){
		perror("sock is deaf and cannot listen !");
		close(sock);
		exit(EXIT_FAILURE);
	}

	// Accepting the next request
	readSock = accept(sock, (struct sockaddr *) &addr, &addr_size);
	if(readSock == -1){
		perror("couldn't accept request.\n");
		close(readSock);
		close(sock);
		exit(EXIT_FAILURE);
	}
}


void bla_spectators(){
	bla(spectatorSock, spectatorReadSock, spectatorAddr);
}

void bla_player(){
	bla(playerSock, playerReadSock, playerAddr);
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
void send_move(char player, char color){
	buffer[0] = player;
	buffer[1] = color;
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
