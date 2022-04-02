#include "csapp.h"
#include "master_body.h"

#define MAX_NAME_LEN 256
#define MST_PORT 2121    /* port number of this server */

int clt_listenfd, clt_connfd;  /* listening and connecting sockets with clients */
int slv1_fd, slv2_fd, slv3_fd; /* client sockets with slave servers */


void sigchld(int sgn) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


void sigintChld(int sgn) {
	/* close slave sockets */
	Close(slv1_fd);
	Close(slv2_fd);
	Close(slv3_fd);
	
	exit(0);
}


void sigintFthr(int sgn) {
	/* send the signal to all children */
	Kill(0, SIGINT);
	while(waitpid(-1, NULL, 0) > 0);
	
	/* close listening socket */
	Close(clt_listenfd);

	/* close connection sockets if existing */
	if (fcntl(clt_connfd, F_GETFD) > 0)
		Close(clt_connfd);

	/* close slave sockets if existing */
	if (fcntl(slv1_fd, F_GETFD) > 0)
		Close(slv1_fd);
	if (fcntl(slv2_fd, F_GETFD) > 0)
		Close(slv2_fd);
	if (fcntl(slv3_fd, F_GETFD) > 0)
		Close(slv3_fd);
	
	exit(0);
}


int main(int argc, char **argv) {
	Signal(SIGCHLD, sigchld);
	Signal(SIGINT, sigintFthr);
	
	socklen_t clt_len;
	struct sockaddr_in clt_addr;
	char clt_ip_string[INET_ADDRSTRLEN];
	char clt_hostname[MAX_NAME_LEN];
	pid_t pid;
	int inc = 1;
	
	/* interprete arguments */
	if (argc != 4) {
		fprintf(stderr, "usage: %s <slv1_hostname> <slv2_hostname> <slv3_hostname>\n", argv[0]);
		exit(0);
	}
	clt_len = (socklen_t) sizeof(clt_addr);
	
	/* connection to slave servers */
	slv1_fd = Open_clientfd(argv[1], MST_PORT+1);
	printf("Master server connected to the slave server %d %s.\n", 1, argv[1]); 
	slv2_fd = Open_clientfd(argv[2], MST_PORT+2);
	printf("Master server connected to the slave server %d %s.\n", 2, argv[2]); 
	slv3_fd = Open_clientfd(argv[3], MST_PORT+3);
	printf("Master server connected to the slave server %d %s.\n", 3, argv[3]); 
	
	/* creation of executants for each slave servers */
	for (int i = 1; i < 4; i++) {
		if ((pid = Fork()) == 0) {
			Signal(SIGCHLD, SIG_DFL);
			Signal(SIGINT, sigintChld);
			
			master_slv_body(i, slv1_fd, slv2_fd, slv3_fd);
			
			exit(0);
		}
	}
	
	/* disconnection from slave servers */
	Close(slv1_fd);
	Close(slv2_fd);
	Close(slv3_fd);
	
	/* open listening socket with clients */
	clt_listenfd = Open_listenfd(MST_PORT);
	
	/* acceptation loop */
	while (1) {
		/* connection to a client */
		while((clt_connfd = Accept(clt_listenfd, (SA *)&clt_addr, &clt_len)) == -1);
		
		/* print informations about the connection */
		Getnameinfo((SA *) &clt_addr, clt_len, clt_hostname, MAX_NAME_LEN, 0, 0, 0);
		Inet_ntop(AF_INET, &clt_addr.sin_addr, clt_ip_string, INET_ADDRSTRLEN);
		printf("Master server connected to the client %s (%s)\n", clt_hostname, clt_ip_string);
		
		inc = master_clt_body(clt_connfd, argv, MST_PORT, inc);
		
		/* disconnection from the client */
		printf("Master server disconnected from the client %s (%s)\n", clt_hostname, clt_ip_string);
		Close(clt_connfd);
	}
	
	exit(0);
}
