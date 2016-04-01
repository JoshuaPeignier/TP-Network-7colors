/* Template of the 7 wonders of the world of the 7 colors assigment. */

/* Cf. http://people.irisa.fr/Anne-Cecile.Orgerie/teaching2015.html  */

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

#define BUFFER_SIZE 500

/** Program entry point */
int main() 
{
	// Initialisation of the game and of the SDL module.
	printf("\n\n  Welcome to the 7 wonders of the world of the 7 colors\n"
	      "  *****************************************************\n\n"
	 "Current board state:\n");
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);



	// Initialising everything necessary variables for the game
	int victory1 = 0; //number of victories of PLAYER1
	int victory2 = 0; //number of victories of PLAYER2
	char mode = 0; //terminal's purpose for the durantion of the game (host player, )
	char previous_mode = 0; // This is here to reset the score, in case we switch between AI player and a real player
	char rematch = 1;
	char difficulty = 0; //to choose between several AIs
	void (*P2)(char) = NULL; // This will be either an AI or a real player

	while(rematch == 1){

		// Selecting a game mode we know
	 	do{
	   		printf("Which mode do you want to play ?\n");
			printf("Please take note that switching mode will reset your score.\n");
	  		printf("1) Player VS. Player \n");
	  		printf("2) Player VS. AI\n");
			printf("3) Spectate an online game\n");
			printf("4) Host an online game\n");
			printf("5) Join an online game\n");
			printf("6) Quit\n");
			scanf("%c",&mode);
			printf("\n\n");
	   	}while(mode < 1 || mode > 6);
		
		// If the mode was changed, we reset the score
		if(previous_mode != mode){
			victory1 = 0;
			victory2 = 0;
		}  
		previous_mode = mode; //then reset the test
		
		// If the 2nd player is an AI, we select its level
		if(mode == 2){
			do{
		   		printf("Which difficulty do you want to play with ?\n");
		  		printf("1) Hard\n");
		  		printf("2) Harder\n");
		  		printf("3) Hardest\n");
		  		printf("4) Hardestest\n");
		  		printf("5) Hardestestest\n");
				scanf("%c",&difficulty);
				printf("\n\n");
		  	}while(difficulty < 1 || difficulty > 5);
		
	
			switch(difficulty){ //chose the AI's stategy that is defined in stategy.c
				case 1: P2 = &improved_random_play; break;
				case 2: P2 = &spider; break;
				case 3: P2 = &greedy; break;
				case 4: P2 = &double_greedy; break;
				case 5: P2 = &mix; break;
			}
		}
		else{
			P2 = &real_play; //defined in strategy.c
		}
	
		// If we are running a game on this computer
		if(mode == 1 || mode == 2){
			char sp = 0;
			// Verifying if we allow spectators
			printf("Allow spectators\n");
			printf("1) Yes\n");
			printf("2) No\n");
			do{
				scanf("%c",&sp);
			}while(sp < 1 || sp > 2);
			
			if(sp == 1){allow_spectators();} //defined in server.c start creating a connection
			else{ban_spectators();} //defined in server.c don't create connections

			// Initialising everything in order to allow spectators to watch
			if(spectate() == 1){bla_spectators();} //if spectator is connected

			// Initialising the game
			init_window();
			random_filling(); //defined in board.c
			update_board(); //defined in display.c
			if(spectate() == 1){send_board_spectators();} // sending your initail bord to spectator (recv in spectator.c)

			// Running the game and printing the scores
			run_game(PLAYER1,&real_play,P2); //defined in stategy.c (PLAYER is a char defined in board.c)
			update_board();
			printf("Player 1 : %d\n",score(PLAYER1)); //defined in auxilary.c
			printf("Player 2 : %d\n",score(PLAYER2));
				printf("\n");
			if(score(PLAYER1) > score(PLAYER2)){victory1++;}
			else{victory2++;}
			printf("\n**************************************\n\n");
			printf("Player 1 : %d victories\n",victory1);
			printf("Player 2 : %d victories\n",victory2);

			if(spectate() == 1){ //sending the spectator off
				spectators_quit(); //defined in server.c
			}


			printf("\n\nDo you want to play again ?\n");
			printf("1) Yes\n");
			printf("2) No\n");
		
			// Asking for a rematch
			do{
				scanf("%c",&rematch);
			}while(rematch < 1 || rematch > 2);
		}
		
		// Spectating an online game
		else if(mode == 3){
			system("./spectate"); //switch to spectator.c executable
		}

		// Hosting an online game
		else if(mode == 4){
			system("./host");
		}

		// Joining an online game as a player
		else if(mode == 5){
			system("./client");
		}

		else if(mode == 6){rematch = 0;}
	
	}
	
	
	
	SDL_Quit();
	return 0; // Everything went well
}
