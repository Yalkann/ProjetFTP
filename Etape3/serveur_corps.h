#ifndef __SERVEUR_CORPS_H__
#define __SERVEUR_CORPS_H__


#define blksize 512             /* maximum number of bytes to send */
#define root "serveurFTP_data/" /* directory of the server data */


/* receive and execute a command */
void serveur_corps(int connfd);

/* send a file to the client */
int serveur_get(int connfd);

#endif
