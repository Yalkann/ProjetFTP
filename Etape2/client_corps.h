#ifndef __CLIENT_CORPS_H__
#define __CLIENT_CORPS_H__


#define blksize 512
#define root "clientFTP_data/"
#define logfile ".log"


void client_corps(int clientfd);

void client_get(int clientfd, char *file, int fread);

void client_help();

#endif
