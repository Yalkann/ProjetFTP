#include "csapp.h"
#include "slaves_body.h"

#define MAX_NAME_LEN 256
#define MST_PORT 2121    /* port number of the master server */
#define NB_PROC 5        /* number of executants */

int listenfd;   /* listening socket */
int mst_connfd; /* connecting socket with the master server */
int clt_connfd; /* connecting socket with clients */


void sigchld(int sgn) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


void sigintChld(int sgn) {
	/* close sockets */
	Close(listenfd);
	Close(mst_connfd);	
	
	/* close client connecting socket if existing */
	if (fcntl(clt_connfd, F_GETFD) > 0)
		Close(clt_connfd);
		
	exit(0);
}


void sigintFthr(int sgn) {
	/* send the signal to all children */
	Kill(0, SIGINT);
	while(waitpid(-1, NULL, 0) > 0);
	
	/* close sockets if existing */
	if (fcntl(listenfd, F_GETFD) > 0)
		Close(listenfd);
	if (fcntl(mst_connfd, F_GETFD) > 0)
		Close(mst_connfd);	

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
	int slv_nb;
	
	/* interprete arguments */
	if (argc != 2) {
		fprintf(stderr, "usage: %s <slv_nb>\n", argv[0]);
		exit(0);
	}
	slv_nb = atoi(argv[1]);
	if (slv_nb < 1 || slv_nb > 3) {
		fprintf(stderr, "error: <slv_nb> must be 1, 2 or 3\n");
		exit(0);		
	}
	clt_len = (socklen_t) sizeof(clt_addr);

	/* open the listening socket */
	listenfd = Open_listenfd(MST_PORT+slv_nb);
	
	/* connection to the master */
	mst_connfd = Accept(listenfd, (SA *)&clt_addr, &clt_len);
	
	/* print informations about the connection */
	Getnameinfo((SA *) &clt_addr, clt_len, clt_hostname, MAX_NAME_LEN, 0, 0, 0);
	Inet_ntop(AF_INET, &clt_addr.sin_addr, clt_ip_string, INET_ADDRSTRLEN);
	printf("Slave server %d connected to the master server %s (%s)\n", slv_nb, clt_hostname, clt_ip_string);
	
	/* creation of the executants */
	for (int i = 0; i < NB_PROC; i++) {
		if ((pid = Fork()) == 0) {
			Signal(SIGCHLD, SIG_DFL);
			Signal(SIGINT, sigintChld);
			
			/* acceptation loop */
			while (1) {
				/* connection to a client */
				while((clt_connfd = Accept(listenfd, (SA *)&clt_addr, &clt_len)) == -1);
				
				/* print informations about the connection */
				Getnameinfo((SA *) &clt_addr, clt_len, clt_hostname, MAX_NAME_LEN, 0, 0, 0);
				Inet_ntop(AF_INET, &clt_addr.sin_addr, clt_ip_string, INET_ADDRSTRLEN);
				printf("Slave server %d connected to the client %s (%s)\n", slv_nb, clt_hostname, clt_ip_string);
				
				slaves_body(slv_nb, clt_connfd, mst_connfd);
				
				/* disconnection from the client */
				printf("Slave server %d disconnected from the client %s (%s)\n", slv_nb, clt_hostname, clt_ip_string);
				Close(clt_connfd);
			}
		}
	}
	
	/* father closes listening socket with clients */
	Close(listenfd);
	
	/* father disconnects from the master server */
	Close(mst_connfd);
	
	while(1)
		Pause();
	exit(0);
}
