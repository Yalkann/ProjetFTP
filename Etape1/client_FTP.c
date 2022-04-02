#include "csapp.h"


#define SRV_PORT 2121 /* port number of the server */


void client_body(int clientfd);

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
	clientfd = Open_clientfd(host, SRV_PORT);
	printf("Client connected to %s.\n", argv[1]); 
	
	client_body(clientfd);
	
	/* disconnection from the server */
	Close(clientfd);
	exit(0);
}

