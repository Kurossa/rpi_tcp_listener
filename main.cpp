/*
 * main.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: mkrzysie
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char** argv) {
	printf("Hello rpi!\n");

    int sockfd, newsockfd, portno;
	unsigned int clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int true_val = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true_val,sizeof(int));
    if (sockfd < 0)
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");

    int nn = 0;
    for (nn = 0; nn < 5; ++nn) {
		listen(sockfd,1);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			 error("ERROR on accept");

		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		printf("Here is the message: %s\n",buffer);
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
    }

    shutdown(newsockfd, 2);
    shutdown(sockfd, 2);

    close(newsockfd);
    close(sockfd);

    return 0;
}

