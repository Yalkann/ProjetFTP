#ifndef __CLIENT_BODY_H__
#define __CLIENT_BODY_H__


#define BLKSIZE 512             /* maximum number of bytes to receive */
#define CLT_ROOT "client_data/" /* directory of the client data */
#define LOGFILE ".log"          /* name of the log file */


/* read and execute a command */
void client_body(int clientfd);

/* receive a file from the server */
void client_get(int clientfd, char *file, int fstart);

/* print authorized commands */
void client_help();

#endif
