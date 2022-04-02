#include "csapp.h"
#include "master_body.h"


void master_slv_body(int i, int slv1_fd, int slv2_fd, int slv3_fd) {
	/* future interconnection between slave servers... */
	return;
}


int master_clt_body(int clt_connfd, char **argv, int mst_port, int inc) {
	size_t n;
	int port;
	
	/* send hostname of the slave server */
	n = strlen(argv[inc]);
	Rio_writen(clt_connfd, &n, sizeof(size_t));
	Rio_writen(clt_connfd, argv[inc], n);
	
	/* send the port number of the slave server */
	port = mst_port + inc;
	Rio_writen(clt_connfd, &port, sizeof(int));

	/* incremente */
	inc++;
	if (inc > 3)
		inc = 1;
	return inc;
}
