#include <stdio.h>  /* printf */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void bla();

void allow_spectators();

void ban_spectators();

int spectate();

void send_board();

void send_move(char player, char color);

void spectators_quit();
