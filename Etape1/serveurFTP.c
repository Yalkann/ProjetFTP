#include "csapp.h"


#define MAX_NAME_LEN 256
#define NB_PROC 5


void serveur_corps(int connfd);
int listenfd;
int connfd;


void sigchld(int sgn) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}


void sigintfils(int sgn) {
	if (fcntl(connfd, F_GETFD) > 0)
		Close(connfd);
	exit(0);
}


void sigintpere(int sgn) {
	Kill(0, SIGINT);
	while(waitpid(-1, NULL, 0) > 0);
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
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		exit(0);
	}
	
	clientlen = (socklen_t) sizeof(clientaddr);
	
	listenfd = Open_listenfd(2121);
	
	for (int i = 0; i < NB_PROC; i++) {
		if ((pid = Fork()) == 0) {
			Signal(SIGCHLD, SIG_DFL);
			Signal(SIGINT, sigintfils);
			
			while (1) {
				while((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen)) == -1);
				
				Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);
				Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);
				printf("Server connected to %s (%s)\n", client_hostname, client_ip_string);
				
				serveur_corps(connfd);
				
				Close(connfd);
			}
		}
	}
	
	Close(listenfd);
	while(1)
		Pause();
	exit(0);
}

