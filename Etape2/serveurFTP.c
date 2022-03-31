#include "csapp.h"
#include "serveur_corps.h"

#define MAX_NAME_LEN 256
#define NB_PROC 5        /* number of executants */

int listenfd, connfd; /* file descriptor for the connection and communication sockets */


void sigchld(int sgn) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


void sigintfils(int sgn) {
	/* close file descriptor if existing */
	if (fcntl(connfd, F_GETFD) > 0)
		Close(connfd);
	
	exit(0);
}


void sigintpere(int sgn) {
	/* send the signal to all children */
	Kill(0, SIGINT);
	while(waitpid(-1, NULL, 0) > 0);
	
	/* close file descriptor if existing */
	if (fcntl(listenfd, F_GETFD) > 0)
		Close(listenfd);
	
	exit(0);
}


int main(int argc, char **argv) {
	Signal(SIGCHLD, sigchld);
	Signal(SIGINT, sigintpere);
	
	socklen_t clientlen;
	struct sockaddr_in clientaddr;
	char client_ip_string[INET_ADDRSTRLEN];
	char client_hostname[MAX_NAME_LEN];
	pid_t pid;
	
	/* interprete arguments */
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		exit(0);
	}
	clientlen = (socklen_t) sizeof(clientaddr);
	
	/* connection to the client */
	listenfd = Open_listenfd(2121);
	
	/* creation of the executants */
	for (int i = 0; i < NB_PROC; i++) {
		if ((pid = Fork()) == 0) {
			Signal(SIGCHLD, SIG_DFL);
			Signal(SIGINT, sigintfils);
			
			/* acceptation loop */
			while (1) {
				/* communication with the client */
				while((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen)) == -1);
				
				/* print informations about the connection */
				Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);
				Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);
				printf("Server connected to %s (%s)\n", client_hostname, client_ip_string);
				
				serveur_corps(connfd);
				
				/* stop communication with the client */
				printf("Server disconnected from %s (%s)\n", client_hostname, client_ip_string);
				Close(connfd);
			}
		}
	}
	
	/* disconnection from the client */
	Close(listenfd);
	while(1)
		Pause();
	exit(0);
}

