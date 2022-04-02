#ifndef __CLIENT_BODY_H__
#define __CLIENT_BODY_H__


#define BLKSIZE 512             /* maximum number of bytes to receive */
#define CLT_ROOT "client_data/" /* directory of the client data */
#define LOGFILE ".log"          /* name of the log file */


/* read an IP adress and port number of a slave server */
void client_mst_body(int clt_fd, char *host, int *port);

/* read and execute a command */
void client_slv_body(int clt_fd);

/* receive a file from the server */
void client_get(int clt_fd, char *file, int fstart);

/* print authorized commands */
void client_help();

#endif
