#ifndef __SERVEUR_CORPS_H__
#define __SERVEUR_CORPS_H__


#define blksize 512
#define root "serveurFTP_data/"


void serveur_corps(int connfd);

void serveur_get(int connfd);

#endif
