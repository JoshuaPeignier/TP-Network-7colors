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
	extern char current_color;

	printf("Waiting for a player to connect...\n");
	bla_player();
	printf("Player connected.\n\n");
	// Initialising the game
	init_window();
	random_filling(); //defined in board.c
	update_board(); //defined in display.c
	send_board_player();

	while(nextturn()){
		update_board(); //display bord
		printf("Player 1 : %d\n",score(PLAYER1));
		printf("Player 2 : %d\n",score(PLAYER2));
		printf("\n");
				
		move = your_turn();
		current_color = move;
		play(PLAYER1,TEMP,move); // The move is done here the game is reconstructed.
		update_board();
		send_move_player();
		if(nextturn()){play(PLAYER2,TEMP,recv_move_player());}
	}

	//end of game update and score
	update_board();
	online_player_quit();
	printf("Player 1 : %d\n",score(PLAYER1));
	printf("Player 2 : %d\n",score(PLAYER2));
	printf("\n");
	printf("Press a key, then Enter to quit");
	getchar();
	
	SDL_Quit();
	return 0;
}
