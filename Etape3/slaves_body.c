#include "csapp.h"
#include "slaves_body.h"


void slaves_body(int slv_nb, int clt_connfd, int mst_connfd) {
	Signal(SIGPIPE, SIG_IGN);
	
	int end = 0; /* boolean */
	int cmd;     /* code command */
	
	while (!end) {
		/* read the command */
		Rio_readn(clt_connfd, &cmd, sizeof(int));
		
		/* interprete the command */
		switch (cmd) {
		case 0:
			end = 1;
			break;
		case 1:
			end = slaves_get(slv_nb, clt_connfd);
			break;
		default:
			printf("Invalid command!\n");
			end = 1;
		}
	}
}

int slaves_get(int slv_nb, int clt_connfd) {
	size_t n;
	char buf[MAXBUF];
	char frep[MAXBUF]; /* file repertory */
	struct stat fstat; /* file stat */
	off_t fsize;       /* file size */
	off_t fsend;       /* file size send */
	int fstart;        /* file index cursor where start to send */
	int fd;            /* file descriptor */
	rio_t rio;
	int pb = 0;
	
	/* read the name of the file to transfer */
	Rio_readn(clt_connfd, &n, sizeof(size_t));
	Rio_readn(clt_connfd, &buf, n);
	printf("File to transfer: %s\n", buf);
	sprintf(frep, SLV_ROOT, slv_nb);
	strcat(frep, buf);
	
	/* take informations about the file */
	if ((fsize = stat(frep, &fstat)) == -1) {
		/* file not found */
		Rio_writen(clt_connfd, &fsize, sizeof(off_t));
	} else {
		/* send the size of the file */
		fsize = fstat.st_size;
		Rio_writen(clt_connfd, &fsize, sizeof(off_t));
		
		/* read where to start the transfer in the file */
		fd = Open(frep, O_RDONLY, 0);
		Rio_readn(clt_connfd, &fstart, sizeof(int));
		lseek(fd, fstart, SEEK_SET);
		fsend = fstart;

		/* send the file content */
		Rio_readinitb(&rio, fd);
		n = Rio_readnb(&rio, buf, BLKSIZE);
		while (!pb && fsend < fsize) {
			fsend += n;
			n = rio_writen(clt_connfd, buf, n);
			if (n != -1)
				n = Rio_readnb(&rio, buf, BLKSIZE);
			else
				pb = 1;
		}
		Close(fd);

		/* print informations about te transfer */
		if (!pb)
			printf("Transfer successfully complete (%ld bytes send).\n", fsize);
		else
			printf("Problem encountered: transfer interrupted! (%ld bytes send)\n", fsend);
	}
	return pb;
}

