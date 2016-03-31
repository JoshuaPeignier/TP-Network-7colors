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

void allow_spectators();

void ban_spectators();

int spectate();

void send_board_spectators();

void send_move_spectators(char player);

void spectators_quit();

void bla_player();

void open_to_online_players();

void close_to_online_players();

int is_playing_online();

void send_board_player();

void send_move_player();

char recv_move_player();

void online_player_quit();
