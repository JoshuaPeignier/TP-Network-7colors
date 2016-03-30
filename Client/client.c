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


// First argument given when you execute the program is the port you want to connect to ; Second is the IP-address of the server ; Third is the string you send to the server
int main(int argc, char* argv[]){

	struct sockaddr_in addr; // The necessary structure to store all pieces of information about the server (IP address, port, etc)
	int sock; // The socket to connect to the server
	socklen_t addr_size = sizeof(struct sockaddr_in);
	char buf[BUF_SIZE]; // The buffer where we store what we get from the server

	// Creating the necessary structure to store the address you're trying to connect to
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[3]);
	inet_pton(AF_INET, inet_ntoa(addr.sin_addr), &addr.sin_addr);




	// Creating the client socket
	sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sock == -1){
		printf("Error : couldn't create the socket !");
		return 0;
	}
	// Connexion test
	if(connect(sock,(struct sockaddr *)&addr,addr_size) == -1){
		printf("Error : couldn't connect to the server socket !\n");
		return 0;
	}






	// Sending bits and trying to get the back
	if(send(sock,argv[4],strlen(argv[4])+1/*BUF_SIZE*/,0) == -1){
		printf("Error : couldn't send any bits.\n");
		return 0;
	}
	//printf("Successfully sent bits.\n");
	recv(sock,buf,BUF_SIZE,0);
	//printf("Received some bits.\n");
	printf("%s\n", buf);

	(void)shutdown(sock, SHUT_RDWR);
	close(sock);
	return 0;
}
