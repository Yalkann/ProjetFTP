#include "csapp.h"
#include "client_body.h"


void client_mst_body(int clt_fd, char *host, int *port) {
	size_t n;
	
	/* read the hostname of the slave server */
	Rio_readn(clt_fd, &n, sizeof(size_t));
	Rio_readn(clt_fd, host, n);
	
	/* read the port number of the slave serve */
	Rio_readn(clt_fd, port, sizeof(int));
}


void client_slv_body(int clt_fd) {
	int end = 0, err = 0; /* booleans */
	int logfd;            /* log file descriptor */
	int argc, cmd;
	char line[MAXLINE];
	char *argv[MAXLINE];
	rio_t rio;
	
	/* read the log file */
	logfd = open(LOGFILE, O_RDONLY);
	if (logfd > 0) {
		Rio_readinitb(&rio, logfd);
		err = Rio_readlineb(&rio, line, MAXLINE);
		Close(logfd);
	}
	
	while (!end) {
		if (!err) {
			/* read a line command */
			printf("ftp> ");
			Fgets(line, MAXLINE, stdin);
			line[strlen(line)-1] = '\0';
		}
		
		/* divide the (interupted) line command */
		argv[0] = strtok(line, " \t");
		argc = 1;
		while ((argv[argc] = strtok(NULL, " \t")) != NULL)
			argc++;
		
		/* interprete the command */
		if (strlen(line) == 0) {
			continue;
		} else if ((strcmp(argv[0], "bye") == 0) && (argc == 1)) {
			cmd = 0;
			Rio_writen(clt_fd, &cmd, sizeof(int));
			end = 1;
		} else if ((strcmp(argv[0], "get") == 0) && err) {
			printf("get %s\n", argv[1]);
			cmd = 1;
			Rio_writen(clt_fd, &cmd, sizeof(int));
			client_get(clt_fd, argv[1], atoi(argv[2]));
		} else if ((strcmp(argv[0], "get") == 0) && !err && (argc == 2)) {
			cmd = 1;
			Rio_writen(clt_fd, &cmd, sizeof(int));
			client_get(clt_fd, argv[1], 0);
		} else {
			client_help();
		}
		
		err = 0;
	}
}


void client_get(int clt_fd, char *file, int fstart) {
	size_t n;
    char buf[MAXBUF];
	rio_t rio;
    char frep[MAXBUF];      /* file repertory */
	off_t fsize;            /* file size */
	off_t fread;			/* file size read */
	int fd;                 /* file descriptor */
	int logfd;              /* log file descriptor */
	struct timespec t1, t2;
	double time, rate;
    
	/* send the name of the file to receive */
	n = strlen(file)+1;
	Rio_writen(clt_fd, &n, sizeof(size_t));
	Rio_writen(clt_fd, file, n);
	strcat(strcpy(frep, CLT_ROOT), file);
	
	clock_gettime(CLOCK_REALTIME, &t1);
	
	/* read the size of the file */
	Rio_readn(clt_fd, &fsize, sizeof(off_t));
	
	if (fsize == -1) {
		/* file not found */
		printf("No such file!\n");
	} else {
		fd = Open(frep, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);		
		
		/* send where to start the transfer in the file */
		Rio_writen(clt_fd, &fstart, sizeof(int));
		lseek(fd, fstart, SEEK_SET);
		fread = fstart;
		
		/* start to read the content of the transfered file */
		Rio_readinitb(&rio, clt_fd);
		if (fsize < BLKSIZE)
			n = Rio_readnb(&rio, buf, fsize);
		else
			n = Rio_readnb(&rio, buf, BLKSIZE);
		
		while (fread < fsize) {
			/* write the content of the file */
			Rio_writen(fd, buf, n);
			
			/* inform the log file about the current transfer */
			fread += n;
			sprintf(buf, "get %s %ld", file, fread);
			logfd = Open(LOGFILE, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);
			Rio_writen(logfd, buf, strlen(buf));
			Close(logfd);
			
			/* read the content of the transfered file */
			if (fsize-fread < BLKSIZE)
				n = Rio_readnb(&rio, buf, fsize-fread);
			else
				n = Rio_readnb(&rio, buf, BLKSIZE);
		}
		Close(fd);
		
		/* transfer succeeds, remove the log file */
		remove(LOGFILE);
		
		/* print informations about te transfer */
		clock_gettime(CLOCK_REALTIME, &t2);
		time = (t2.tv_sec - t1.tv_sec) + 1e-9*(t2.tv_nsec - t1.tv_nsec);
		rate = (fsize / 100) / time;
		printf("Transfer successfully complete.\n");
		printf("%ld bytes received in %lf seconds (%lf Kbytes/s).\n", fsize, time, rate);
	}
}


void client_help() {
	printf("File transfer protocol commands:\n");
	printf("   > get <file>     receive the file\n");
	printf("   > bye            quit\n");
	printf("   > help           print this help\n");
}
