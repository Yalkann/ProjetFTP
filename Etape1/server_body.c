#include "csapp.h"


#define ROOT "server_data/"


void server_body(int connfd) {
	size_t n;
	char *buf;
	char fname[MAXBUF]; /* file name */
	char frep[MAXBUF];  /* file repertory */
	struct stat fstat;  /* file stat */
	off_t fsize;        /* file size */
	int fd;             /* file descriptor */
	
	/* read the name of the file to transfer */
	Rio_readn(connfd, &n, sizeof(size_t));
	Rio_readn(connfd, &fname, n);
	printf("Server received %u bytes : %s\n", (unsigned int) n, fname);
	strcat(strcpy(frep, ROOT), fname);
	
	/* take informations about the file */
	if ((fsize = stat(frep, &fstat)) == -1) {
		/* file not found */
		Rio_writen(connfd, &fsize, sizeof(off_t));
	} else {
		/* send the size of the file */
		fsize = fstat.st_size;
		Rio_writen(connfd, &fsize, sizeof(off_t));
		
		/* send the file content */
		fd = Open(frep, O_RDONLY, 0);
		buf = Malloc(fsize);
		n = Rio_readn(fd, buf, fsize);
		Rio_writen(connfd, buf, n);
		Free(buf);
		Close(fd);

		/* print informations about te transfer */
		printf("Transfer successfully complete (%ld bytes send).\n", fsize);
	}
}

