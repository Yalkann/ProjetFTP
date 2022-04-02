#ifndef __SLAVES_BODY_H__
#define __SLAVES_BODY_H__


#define BLKSIZE 512              /* maximum number of bytes to send */
#define SLV_ROOT "slave%d_data/" /* directory of the slave server data */


/* receive and execute a command */
void slaves_body(int slv_nb, int clt_connfd, int mst_connfd);

/* send a file to the client */
int slaves_get(int slv_nb, int connfd);

#endif
