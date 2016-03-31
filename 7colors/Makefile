all: 7colors spectate client

7colors: 7colors.o
	gcc board.o server.o display.o auxiliary.o strategies.o 7colors.o -o 7colors `sdl-config --cflags --libs` 

spectate: spectate.o
	gcc board.o server.o display.o auxiliary.o strategies.o spectate.o -o spectate `sdl-config --cflags --libs` 

client: client.o
	gcc board.o server.o display.o auxiliary.o strategies.o client.o -o client `sdl-config --cflags --libs` 

7colors.o: 7colors.c board.c auxiliary.c strategies.c display.c server.c
	gcc -Wall -Werror -Wextra -lSDL_image board.c -c -g
	gcc -Wall -Werror -Wextra -lSDL_image server.c -c -g
	gcc -Wall -Werror -Wextra -lSDL_image display.c -c -g
	gcc -Wall -Werror -Wextra -lSDL_image auxiliary.c -c -g
	gcc -Wall -Werror -Wextra -lSDL_image strategies.c -c -g
	gcc -Wall -Werror -Wextra -lSDL_image 7colors.c -c -g

spectate.o: spectate.c board.c auxiliary.c strategies.c display.c
	gcc -Wall -Werror -Wextra -lSDL_image spectate.c -c -g

client.o: client.c board.c auxiliary.c strategies.c display.c
	gcc -Wall -Werror -Wextra -lSDL_image client.c -c -g

clean:
	rm -rf *.o

mrproper: clean
	rm -rf 7colors
