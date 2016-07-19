#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

#include "tslib.h"

#define BUFSIZE 1024

int main(int argc, char **argv) {
    int sockfd, portno, n;
    socklen_t serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];


    struct timeval starttime;
    struct timeval endtime;

    /* check command line arguments */
    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { 
        perror("socket");
	return 1;
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    memcpy((char *)server->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    if(inet_aton(hostname, &serveraddr.sin_addr) == 0) {
    	printf("inet_aton\n");
	return 1;
    }
    
    timecmd_t command;
    command.cmd = CMD_GETTIME;
    command.id = 1;

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, &command, sizeof(timecmd_t), 0, (struct sockaddr *) &serveraddr, serverlen);
    printf("n %d\n", n);
    gettimeofday(&starttime, NULL);
    if (n < 0) {
      perror("sendto");
      return 1;
    }
    
    /* print the server's reply */
    n = recvfrom(sockfd, buf, sizeof(timecmd_t), 0, (struct sockaddr *) &serveraddr, &serverlen);
    gettimeofday(&endtime, NULL);
    if (n < 0) {
      perror("recvfrom");
      return 1;
    }

    timecmd_t *tmpcmd = (timecmd_t *) &buf[0];
    timecmd_print(tmpcmd);
    printf("Net latency %d usec\n", deltausec(starttime, endtime));

    return 0;
}
