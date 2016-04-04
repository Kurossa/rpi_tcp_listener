/*
 * tcp_connection.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: mariusz
 */

//#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "tcp_connection.hpp"
#include "../utilities/app_logger.hpp"

tcpConStatus_t tcpConnection::connect(void) {
	tcpConStatus_t status = TCP_ERROR_GENERAL;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int true_val = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));
	if (sockfd < 0) {
		status = TCP_ERROR_OPENING_PORT;
		logPrintf(ERROR_LOG, "ERROR opening socket");
	} else {
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);

		int n = 0;
		n = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

		if (n < 0) {
			status = TCP_ERROR_ON_BINDING;
			logPrintf(ERROR_LOG, "ERROR on binding");
		}
	}
	return status;
}

tcpConStatus_t tcpConnection::receive(char* rcv_buffer, int rcv_buffer_len) {

	tcpConStatus_t status = TCP_ERROR_GENERAL;
	int n = 0;

	listen(sockfd, 1);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		logPrintf(ERROR_LOG, "ERROR on accept");
		status = TCP_ERROR_ACCEPT_SOCKET;
	}

	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0) {
		logPrintf(ERROR_LOG, "ERROR reading from socket");
		status = TCP_ERROR_READ_FROM_SOCKET;
	}

	printf("Here is the message: %s\n", buffer);
	n = write(newsockfd, "I got your message", 18);
	if (n < 0) {
		logPrintf(ERROR_LOG, "ERROR writing to socket");
		status = TCP_ERROR_WRITE_TO_SOCKET;
	}
	return status;
}

void tcpConnection::disconnect() {
	shutdown(newsockfd, 2);
	shutdown(sockfd, 2);

	close(newsockfd);
	close(sockfd);
}
