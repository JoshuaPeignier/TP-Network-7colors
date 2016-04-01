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
#define MAX_SPECTATORS 16

// Initialising everything necessary for online interactions
socklen_t addr_size;
int yes = 1;
char buffer[BUFFER_SIZE];
char current_color;

//Socket and address dedicated to the spectator
int allow_spectate = 0;
int spectatorSock; // The socket we will open
int spectatorReadSock; // The socket where we will store the one returned by accept()
struct sockaddr_in spectatorAddr; // The variable which contains each piece of information about the current client (IP address, port, etc)
//Same here for a player who plays online
int play_online = 0;
int playerSock; // The socket we will open
int playerReadSock; // The socket where we will store the one returned by accept()
struct sockaddr_in playerAddr; // The variable which contains each piece of information about the current client (IP address, port, etc)

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

void send_board_spectators(){ //at the bigginig must send the initial bord
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
void send_move_spectators(char player){
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


// Bind-Listen-Accept
void bla_player(){
	// Creating the necessary structure to store the address you want to connect to
	addr_size = sizeof(struct sockaddr_in);
	memset(&playerAddr, 0, sizeof playerAddr); //memorizes a constant (thanks wikipedia)
	playerAddr.sin_family = AF_INET;
	playerAddr.sin_port = htons(7778); // the port that will be used one the connection is etablished (hopefully wll bring luck)
	playerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Creating the socket
	playerSock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); //!!! AF est une generalisation de PF (essayons de remplacer)
	if(playerSock == -1){
		perror("couldn't create the socket !\n");
		exit(EXIT_FAILURE);
	}

	// Avoiding the "address already in use" error
	if(setsockopt(playerSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("couldn't avoid the 'address already in use' error");
		exit(EXIT_FAILURE);
	}

	// Binding the socket to previous address
	if( bind(playerSock, (struct sockaddr*) &playerAddr, addr_size) == -1){
		perror("couldn't bind the socket and the adress !\n");
		close(playerSock);
		exit(EXIT_FAILURE);
	}

	// Listening (finding and listing plugs)
	if(listen(playerSock, 1024) == -1){
		perror("sock is deaf and cannot listen !");
		close(playerSock);
		exit(EXIT_FAILURE);
	}

	// Accepting the next request
	playerReadSock = accept(playerSock, (struct sockaddr *) &playerAddr, &addr_size);
	if(playerReadSock == -1){
		perror("couldn't accept request.\n");
		close(playerReadSock);
		close(playerSock);
		exit(EXIT_FAILURE);
	}
}


//functions will be used in 7color.c to decide whever to wait for a player or not
void open_to_online_players(){play_online = 1;}
void close_to_online_players(){play_online = 2;}
int is_playing_online(){return play_online;}

void send_board_player(){ //at the bigginig must send the initial bord
	copy_board(buffer);
	buffer[BOARD_SIZE*BOARD_SIZE] = '\0';

	if(send(playerReadSock, buffer, BUFFER_SIZE, 0) == -1){
		perror("couldn't send anything");
		close(playerReadSock);
		close(playerSock);
		exit(EXIT_FAILURE);
	}
}

//used after each game turn, with it the observer can update his game.
void send_move_player(){
	buffer[0] = current_color;
	buffer[1] = '\0';
	if(send(playerReadSock, buffer, BUFFER_SIZE, 0) == -1){
		perror("couldn't send anything");
		close(playerReadSock);
		close(playerSock);
		exit(EXIT_FAILURE);
	}
}

char recv_move_player(){
	if(recv(playerReadSock, buffer, BUFFER_SIZE, 0) == -1){
		perror("couldn't receive anything");
		close(playerReadSock);
		close(playerSock);
		exit(EXIT_FAILURE);
	}
	return buffer[0];
}

void online_player_quit(){
	shutdown(playerSock, SHUT_RDWR);
	shutdown(playerReadSock, SHUT_RDWR);
	close(playerReadSock);
	close(playerSock);
}
