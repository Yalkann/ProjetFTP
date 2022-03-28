#include "csapp.h"
#include "serveur_corps.h"


void serveur_corps(int connfd) {
	int end = 0;
	int cmd;
	
	while (!end) {
		/* read the command */
		Rio_readn(connfd, &cmd, sizeof(int));
		
		/* interprete the command */
		switch (cmd) {
		case 0:
			end = 1;
			break;
		case 1:
			serveur_get(connfd);
			break;
		default:
			printf("Invalid command!\n");
			end = 1;
		}
	}
}

void serveur_get(int connfd) {
	size_t n;
	char buf[MAXBUF];
	char frep[MAXBUF]; /* file repertory */
	struct stat fstat; /* file stat */
	off_t fsize;       /* file size */
	int fsend;         /* file size already send */
	int fd;            /* file descriptor */
	rio_t rio;
	
	/* read the name of the file to transfer */
	Rio_readn(connfd, &n, sizeof(size_t));
	Rio_readn(connfd, &buf, n);
	printf("File to transfer: %s\n", buf);
	strcat(strcpy(frep, root), buf);
	
	/* take informations about the file */
	if ((fsize = stat(frep, &fstat)) == -1) {
		/* file not found */
		Rio_writen(connfd, &fsize, sizeof(off_t));
	} else {
		/* send the size of the file */
		fsize = fstat.st_size;
		Rio_writen(connfd, &fsize, sizeof(off_t));
		
		/* read where to start the transfer in the file */
		fd = Open(frep, O_RDONLY, 0);
		Rio_readn(connfd, &fsend, sizeof(int));
		lseek(fd, fsend, SEEK_SET);

		/* send the file content */
		Rio_readinitb(&rio, fd);
		n = Rio_readnb(&rio, buf, blksize);
		while (n > 0) {
			Rio_writen(connfd, buf, n);
			n = Rio_readnb(&rio, buf, blksize);
		}
		Close(fd);

		/* print informations about te transfer */
		printf("Transfer successfully complete (%ld bytes send).\n", fsize);
	}
}

