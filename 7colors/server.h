#include <stdio.h>  /* printf */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

//void bla(int sock, int readSock, struct sockaddr_in addr);

void bla_spectators();

//void bla_player();

void allow_spectators();

void ban_spectators();

int spectate();

void send_board();

void send_move(char player);

void spectators_quit();
