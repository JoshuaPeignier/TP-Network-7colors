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
#include "server.h"

#define BUFFER_SIZE 1000


int main(){
	SDL_Init(SDL_INIT_EVERYTHING);
	char move = 0;
	char sp = '0';
	extern char current_color;

	printf("Waiting for a player to connect...\n");
	bind_listen_player();
	printf("Player connected.\n\n");
	
	// Verifying if we allow spectators
	printf("Allow spectators\n");
	printf("1) Yes\n");
	printf("2) No\n");
	do{
		scanf("%c",&sp);
	}while(sp < '1' || sp > '2');

	if(sp == '1'){allow_spectators();} //defined in server.c start creating a connection
	else{ban_spectators();} //defined in server.c don't create connections

	// Initialising everything in order to allow spectators to watch
	if(spectate() == 1){bind_listen_spectators(); /*accept_new_spectators();*/} //if spectator is connected
	
	// Initialising the game
	init_window();
	random_filling(); //defined in board.c
	update_board(); //defined in display.c
	send_board_player();
	if(spectate() == 1){send_board_spectators();} // sending your initail bord to spectator (recv in spectator.c)

	while(nextturn()){
		update_board(); //display bord
		printf("Player 1 : %d\n",score(PLAYER1));
		printf("Player 2 : %d\n",score(PLAYER2));
		printf("\n");
		move = your_turn();
		current_color = move;
		play(PLAYER1,TEMP,move); // The move is done here the game is reconstructed.
		update_board();
		send_move_spectators(PLAYER1);
		send_move_player();
		if(nextturn()){
			move = recv_move_player();
			play(PLAYER2,TEMP,move);
			send_move_spectators(PLAYER2);
			}
	}

	//end of game update and score
	update_board();
	online_player_quit();
	printf("Player 1 : %d\n",score(PLAYER1));
	printf("Player 2 : %d\n",score(PLAYER2));
	printf("\n");
	
	if(spectate() == 1){ //sending the spectator off
		spectators_quit(); //defined in server.c
	}
	
	printf("Press a key, then Enter to quit");
	getchar();
	
	SDL_Quit();
	return 0;
}
