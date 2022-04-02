#ifndef __MASTER_BODY_H__
#define __MASTER_BODY_H__


/* receive and execute a command from a slave to other slaves servers */
void master_slv_body(int i, int slv1_fd, int slv2_fd, int slv3_fd);

/* send to the client an IP adress and port number of a slave server */
int master_clt_body(int clt_connfd, char **argv, int mst_port, int inc);

#endif
