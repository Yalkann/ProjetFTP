#ifndef __SERVEUR_BODY_H__
#define __SERVEUR_BODY_H__


#define blksize 512         /* maximum number of bytes to send */
#define root "server_data/" /* directory of the server data */


/* receive and execute a command */
void server_body(int connfd);

/* send a file to the client */
int server_get(int connfd);

#endif
