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

	// Creating the socket
	int sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock == -1){
		printf("Error : couldn't create the socket !\n");
		return 0;
	}
	printf("Successfully created a socket !\n");

	// Creating the necessary structure to stock the address you want to connect to
	struct sockaddr_in addr;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t peer_addr_size = sizeof(struct sockaddr_in);
	
	char buf[BUF_SIZE];

	// Binding the socket to previous address
	if(bind(sock,(struct sockaddr*) &addr, sizeof(struct sockaddr_in)) == -1){
		printf("Error : couldn't bind the socket and the adress !\n");
		return 0;
	}
	printf("Successfully bound socket and address on the port %d!\n",atoi(argv[2]));

	// Listening
	if(listen(sock,1024) == -1){
		printf("Error : sock is deaf and cannot listen !");
		return 0;
	}
	printf("The server socket is currently listening !\n");


	// Entering the accept-loop
	int consocket = accept(sock, NULL, NULL/*(struct sockaddr *) &addr, &peer_addr_size*/);
	while(consocket){
		printf("Trying to accept a new request.\n");
		if(/*accept(sock, (struct sockaddr *) &addr, &peer_addr_size)*/consocket==-1){
			printf("Error : couldn't accept request.\n");
		}
		printf("Successfully accepted the request.\n");
		recv(sock,buf,BUF_SIZE,0);
		printf("Successfully received some bits from %s.\n", inet_ntoa(addr.sin_addr));
		//printf("%s", inet_ntoa(addr.sin_addr));
		send(sock,buf,BUF_SIZE,0);
		printf("Successfully send the bits back.\n");
		consocket = accept(sock, (struct sockaddr *) &addr, &peer_addr_size);	
	}

	close(sock);
	return 0;
}
