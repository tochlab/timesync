#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tslib.h"

#define BUFSIZE 1024

int main() {
	socklen_t clientlen; /*  byte size of client's address */
	struct sockaddr_in serveraddr; /*  server's addr */
	struct sockaddr_in clientaddr; /*  client addr */
	char buf[BUFSIZE]; /*  message buf */
	ssize_t n; /*  message byte size */
	char *hostaddrp; /*  dotted decimal host addr string */
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	memset((char *) &serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(1981);

	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
		perror("bind");
	}

	clientlen = sizeof(clientaddr);
	while (1) {
		memset(buf, 0, BUFSIZE);
		n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
		if (n < 0) {
			perror("recvfrom");
			return 1;
		}

		if( n != sizeof(timecmd_t)) {
		    printf("readed buffer broken got %zd need %zd\n", n, sizeof(timecmd_t));
		}

		hostaddrp = inet_ntoa(clientaddr.sin_addr);
    		
		if (hostaddrp == NULL) {
			perror("inet_ntoa\n");
			return 1;
		}

		printf("server received datagram from %s\n", hostaddrp);
		timecmd_t *command = (timecmd_t *) &buf[0];
		switch(command->cmd) {
			case CMD_GETTIME: {
				struct timeval tv;
				printf("CMD_GETTIME\n");
				gettimeofday(&tv, NULL);
				command->version = 0;
				command->sec = tv.tv_sec;
				command->usec = tv.tv_usec;
				timecmd_print(command);
				}
			break;
			default:
				printf("Unknown command\n");
			break;
		}

	        n = sendto(sockfd, buf, sizeof(timecmd_t), 0, (struct sockaddr *) &clientaddr, clientlen);
	        if (n < 0) {
		        perror("ERROR in sendto");
			return 1;
		}
	}
	return 0;
}
