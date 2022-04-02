#include "csapp.h"
#include "client_body.h"


#define MST_PORT 2121 /* port number of the master server */

int clt_fd; /* client socket */


void sigint(int sgn) {
	/* close client socket if existing */
	if (fcntl(clt_fd, F_GETFD) > 0)
		Close(clt_fd);
	
	exit(0);
}


int main(int argc, char **argv) {
	Signal(SIGINT, sigint);
	
	char slv_host[MAXLINE];
	int slv_port;
	
	/* interprete arguments */
	if (argc != 2) {
		fprintf(stderr, "usage: %s <host>\n", argv[0]);
		exit(0);
	}
	
	/* connection to the master server */
	clt_fd = Open_clientfd(argv[1], MST_PORT);
	printf("Client connected to the master server %s.\n", argv[1]); 
	
	client_mst_body(clt_fd, slv_host, &slv_port);
	
	/* disconnection from the master server */
	Close(clt_fd);
	printf("Client disconnected from the master server %s.\n", argv[1]);

	/* connection to the slave server */
	clt_fd = Open_clientfd(slv_host, slv_port);
	printf("Client connected to the slave server %s.\n", slv_host); 
	
	client_slv_body(clt_fd);
	
	/* disconnection from the slave server */
	Close(clt_fd);
	printf("Client disconnected from the slave server %s.\n", slv_host);
	exit(0);
}

