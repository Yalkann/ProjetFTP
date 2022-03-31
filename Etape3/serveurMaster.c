#include "csapp.h"

#define port 2121
#define MAX_NAME_LEN 256
#define NB_SLAV 5
#define NB_PROC 5

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
	int pipes[NB_SLAV][2];
	int ports[NB_SLAV];
	int slaveport;
	
	if (argc != 1) {
		fprintf(stderr, "usage: %s\n", argv[0]);
		exit(0);
	}
	
	clientlen = (socklen_t) sizeof(clientaddr);
	
	for (int i = 1; i <= NB_SLAV; i++) {
		if ((pid = Fork()) == 0) {
			execvp("serveurFTP", [itoa(port+i)]);
		}
		ports[i-1] = Open_clientfd(host, port+i);
	}
	
	listenfd = Open_listenfd(port);
	
	for (int i = 0; i < NB_PROC; i++) {
		if ((pid = Fork()) == 0) {
			Signal(SIGCHLD, SIG_DFL);
			Signal(SIGINT, sigintfils);
			
			while (1) {
				while((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen)) == -1);
				
				Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);
				Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);
				printf("Server connected to %s (%s)\n", client_hostname, client_ip_string);
				
				slaveport = ;
				Rio_writen(connfd, &slaveport, sizeof(int));
				
				printf("Server disconnected from %s (%s)\n", client_hostname, client_ip_string);
				Close(connfd);
			}
		}
	}
	
	Close(listenfd);
	while(1)
		Pause();
	exit(0);
}

