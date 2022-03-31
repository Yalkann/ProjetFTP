#include "csapp.h"


#define root "client_data/"


void client_body(int clientfd) {
	size_t n;
    char *buf;
    char fname[MAXBUF];     /* file name */
	off_t fsize;            /* file size */
    char frep[MAXBUF];      /* file repertory */
	int fd;                 /* file descriptor */
	struct timespec t1, t2;
	double time, rate;
	
	/* read the file name to receive */
	printf("ftp> ");
	Fgets(fname, MAXLINE, stdin);
	fname[strlen(fname)-1] = '\0';
    
	/* send the file name */
	n = strlen(fname)+1;
	Rio_writen(clientfd, &n, sizeof(size_t));
	Rio_writen(clientfd, fname, n);
	strcat(strcpy(frep, root), fname);
	
	clock_gettime(CLOCK_REALTIME, &t1);
	
	/* read the size of the file */
	Rio_readn(clientfd, &fsize, sizeof(off_t));
	
	if (fsize == -1) {
		/* file not found */
		printf("No such file!\n");
	} else {
		fd = Open(frep, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);

		/* read the file */
		buf = Malloc(fsize);
		n = Rio_readn(clientfd, buf, fsize);
		Rio_writen(fd, buf, n);
		Free(buf);
		Close(fd);
		
		/* print informations about te transfer */
		clock_gettime(CLOCK_REALTIME, &t2);
		time = (t2.tv_sec - t1.tv_sec) + 1e-9*(t2.tv_nsec - t1.tv_nsec);
		rate = (fsize / 100) / time;
		printf("Transfer successfully complete.\n");
		printf("%ld bytes received in %lf seconds (%lf Kbytes/s).\n", fsize, time, rate);
	}
}

