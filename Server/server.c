#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 500
/*
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};


*/



// First argument given when you execute the program is the port you want to connect to
int main(int argc, char* argv[]){


	int sock; // The socket we will open
	int readSock; // The socket where we will store the one returned by accept()
	struct sockaddr_in addr; // The variable which contains each piece of information about the current client (IP address, port, etc)
	char buffer[BUFFER_SIZE]; // The buffer, where we store the received data
	socklen_t addr_size = sizeof(struct sockaddr_in);

	// Creating the necessary structure to store the address you want to connect to
	memset(&addr, 0, sizeof addr); //function discovereed on wikipedia that stores a constant
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);





	// Creating the socket
	sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); //!!! euh josh ne faut il pas utiliser AF_INET???
	if(sock == -1){
		perror("couldn't create the socket !\n"); // error for socket
		exit(EXIT_FAILURE);
	}

	// Binding the socket to previous address
	if( bind(sock, (struct sockaddr*) &addr, addr_size) == -1){
		perror("couldn't bind the socket and the adress !\n"); //error for bind
		close(sock);
		exit(EXIT_FAILURE);
	}

	// Listening to discover potential plugs
	if(listen(sock, 1024) == -1){
		perror("sock is deaf and cannot listen !"); //error for listen
		close(sock);
		exit(EXIT_FAILURE);
	}



	// Entering the accept-loop
	while(readSock){
		// Accepting the next request is listening queu
		readSock = accept(sock, (struct sockaddr *) &addr, &addr_size);
		if(readSock == -1){
			perror("couldn't accept request.\n"); //error for accept
			close(readSock);
			close(sock);
			exit(EXIT_FAILURE);
		}

		// Receiving bits
		if(recv(readSock, buffer, BUFFER_SIZE, 0) == -1){
			perror("couldn't receive anything"); //error for recv
			close(readSock);
			close(sock);
			exit(EXIT_FAILURE);
		}
		printf("Successfully received some bits from %s.\n", inet_ntoa(addr.sin_addr));

		// Emitting them again
		if(send(readSock, buffer, BUFFER_SIZE, 0) == -1){
			perror("couldn't send anything"); //error for send
			close(readSock);
			close(sock);
			exit(EXIT_FAILURE);
		}
		printf("Successfully send the bits back.\n");
	}

	close(sock);
	return 0;
}
