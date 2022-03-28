#include "csapp.h"
#include "client_corps.h"

int clientfd;


void sigint(int sgn) {
	if (fcntl(clientfd, F_GETFD) > 0)
		Close(clientfd);
	exit(0);
}


int main(int argc, char **argv) {
	Signal(SIGINT, sigint);
	
	char *host;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <host>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	
	clientfd = Open_clientfd(host, 2121);
	printf("Client connected to %s.\n", argv[1]); 
	
	client_corps(clientfd);
	
	Close(clientfd);
	printf("Client disconnected from %s.\n", argv[1]);
	exit(0);
}

