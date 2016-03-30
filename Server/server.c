#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 500
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

	int sock;
	int connect = 1;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buf[BUF_SIZE];

	addr_size = sizeof(struct sockaddr_in);



	// Creating the socket
	sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock == -1){
		perror("couldn't create the socket !\n");
		exit(EXIT_FAILURE);
	}
	printf("Successfully created a socket !\n");


	// Creating the necessary structure to stock the address you want to connect to
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding the socket to previous address
	if(bind(sock,(struct sockaddr*) &addr, addr_size) == -1){
		perror("couldn't bind the socket and the adress !\n");
		close(sock);
		exit(EXIT_FAILURE);
	}
	printf("Successfully bound socket and address on the port %d!\n",atoi(argv[2]));

	// Listening
	if(listen(sock,1024) == -1){
		perror("sock is deaf and cannot listen !");
		close(sock);
		exit(EXIT_FAILURE);
	}
	printf("The server socket is currently listening !\n");



	// Entering the accept-loop
	while(connect){
		connect = accept(sock, /*NULL, NULL*/(struct sockaddr *) &addr, &addr_size);
		printf("Trying to accept a new request.\n");
		if(/*accept(sock, (struct sockaddr *) &addr, &peer_addr_size)*/connect==-1){
			perror("couldn't accept request.\n");
			close(connect);
			close(sock);
			exit(EXIT_FAILURE);
		}

		printf("Successfully accepted the request.\n");
		recv(sock,buf,BUF_SIZE,0);
		printf("Successfully received some bits from %s.\n", inet_ntoa(addr.sin_addr));
		//printf("%s", inet_ntoa(addr.sin_addr));
		send(sock,buf,BUF_SIZE,0);
		printf("Successfully send the bits back.\n");	
	}

	close(sock);
	return 0;
}
