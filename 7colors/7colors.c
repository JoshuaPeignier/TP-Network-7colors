/* Template of the 7 wonders of the world of the 7 colors assigment. */

/* Cf. http://people.irisa.fr/Anne-Cecile.Orgerie/teaching2015.html  */

#include <stdio.h>  /* printf */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include "board.h"
#include "display.h"
#include "auxiliary.h"
#include "strategies.h"



/** Program entry point */
int main() 
{
   printf("\n\n  Welcome to the 7 wonders of the world of the 7 colors\n"
	      "  *****************************************************\n\n"
	 "Current board state:\n");
   srand(time(NULL));
   SDL_Init(SDL_INIT_EVERYTHING);

   int victory1 = 0;
   int victory2 = 0;
   int mode = 0;
   int previous_mode = 0; // This is here to reset the score, in case we switch between AI player and a real player
   int rematch = 1;
   int difficulty = 0;
   void (*P2)(char) = NULL; // This will be either an AI or a real player



   while(rematch == 1){

	// Selecting a game mode
	previous_mode = mode;
 	do{
   		printf("Which mode do you want to play ?\n");
		printf("Please take note that switching mode will reset your score.\n");
  		printf("1) Player VS. Player \n");
  		printf("2) Player VS. AI\n");
		scanf("%d",&mode);
		printf("\n\n");
   	}while(mode < 1 || mode > 2);
	
	// If the mode was changed, we reset the score
	if(previous_mode != mode){
		victory1 = 0;
		victory2 = 0;
	}  

	// If the 2nd player is an AI, we select its level
	if(mode == 2){
		do{
	   		printf("Which difficulty do you want to play with ?\n");
	  		printf("1) Hard\n");
	  		printf("2) Harder\n");
	  		printf("3) Hardest\n");
	  		printf("4) Hardestest\n");
	  		printf("5) Hardestestest\n");
			scanf("%d",&difficulty);
			printf("\n\n");
	  	}while(difficulty < 1 || difficulty > 5);
	

		switch(difficulty){
			case 1: P2 = &improved_random_play; break;
			case 2: P2 = &spider; break;
			case 3: P2 = &greedy; break;
			case 4: P2 = &double_greedy; break;
			case 5: P2 = &mix; break;
		}
	}
	else{
		P2 = &real_play;
	}

	// Initialising the game
	init_window();
	random_filling();
	update_board();

	// Running the game and printing the scores
	run_game(PLAYER1,&real_play,P2);
	update_board();
	printf("Player 1 : %d\n",score(PLAYER1));
	printf("Player 2 : %d\n",score(PLAYER2));
	printf("\n");
	if(score(PLAYER1) > score(PLAYER2)){victory1++;}
	else{victory2++;}
	printf("\n**************************************\n\n");
	printf("Player 1 : %d victories\n",victory1);
	printf("Player 2 : %d victories\n",victory2);
	printf("\n\nDo you want to play again ?\n");
	printf("1) Yes\n");
	printf("2) No\n");

	// Asking for a rematch
	do{
		scanf("%d",&rematch);
	}while(rematch < 1 || rematch > 2);
   }



   SDL_Quit();
   return 0; // Everything went well
}
