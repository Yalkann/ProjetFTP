#include "csapp.h"
#include "client_corps.h"

int clientfd; /* file descriptor for the client sockets */


void sigint(int sgn) {
	/* close file descriptor if existing */
	if (fcntl(clientfd, F_GETFD) > 0)
		Close(clientfd);
	
	exit(0);
}


int main(int argc, char **argv) {
	Signal(SIGINT, sigint);
	
	/* interprete arguments */
	char *host;
	if (argc != 2) {
		fprintf(stderr, "usage: %s <host>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	
	/* connection to the server */
	clientfd = Open_clientfd(host, 2121);
	printf("Client connected to %s.\n", argv[1]); 
	
	client_corps(clientfd);
	
	/* disconnection from the server */
	Close(clientfd);
	printf("Client disconnected from %s.\n", argv[1]);
	exit(0);
}

