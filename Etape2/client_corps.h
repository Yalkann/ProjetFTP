#ifndef __CLIENT_CORPS_H__
#define __CLIENT_CORPS_H__


#define blksize 512            /* maximum number of bytes to receive */
#define root "clientFTP_data/" /* directory of the client data */
#define logfile ".log"         /* name of the log file */


/* read and execute a command */
void client_corps(int clientfd);

/* receive a file from the server */
void client_get(int clientfd, char *file, int fread);

/* print authorized commands */
void client_help();

#endif
