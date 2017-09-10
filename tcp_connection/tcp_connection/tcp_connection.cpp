/*
 * tcp_connection.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: mariusz
 */

#include "tcp_connection.h"
#include <utilities/logger.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

tcpConStatus_t tcpConnection::changeIp(const char* ip, const char* mask, const char* gateway) {
    int status = TCP_NO_ERROR;
    struct ifreq ifr;
    const char * name = "eth0";
    int fd =0;

    if (-1 == (fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP))) {
        return TCP_ERROR_CREATE_FD;
    }

    /* Set device's name */
    strncpy(ifr.ifr_name, name, IFNAMSIZ);

    /* Set IP */
    ifr.ifr_addr.sa_family = AF_INET;
    struct sockaddr_in* addr = (struct sockaddr_in*) &ifr.ifr_addr;
    inet_pton(AF_INET, ip, &addr->sin_addr);
    if (-1 == ioctl(fd, SIOCSIFADDR, &ifr)) {
        close(fd);
        return TCP_ERROR_SET_IP;
    }

    /* Set netmask */
    addr = (struct sockaddr_in *) &(ifr.ifr_netmask);
    inet_pton(AF_INET, mask, &addr->sin_addr);
    if (-1 == ioctl(fd, SIOCSIFNETMASK, &ifr)) {
        close(fd);
        return TCP_ERROR_SET_MASK;
    }

    //ioctl(fd, SIOCGIFFLAGS, &ifr);
    //strncpy(ifr.ifr_name, name, IFNAMSIZ);
    /* Set flags */
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
    if (-1 == ioctl(fd, SIOCSIFFLAGS, &ifr)) {
        close(fd);
        return TCP_ERROR_SET_FLAGS;
    }

    status = setRoute(fd, gateway);

    close(fd);
    return (tcpConStatus_t)status;
}

tcpConStatus_t tcpConnection::setRoute(int sockfd, const char *gateway_addr) {
    tcpConStatus_t status = TCP_NO_ERROR;

    struct rtentry route;
    memset(&route, 0, sizeof(route));
    struct sockaddr_in *addr = (struct sockaddr_in*) &route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(gateway_addr);
    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_metric = 100;

    if (0 != ioctl(sockfd, SIOCADDRT, &route)) {
        status = TCP_ERROR_SET_ROUTE;
    }
    return status;
}

tcpConStatus_t tcpConnection::connect(void) {
    tcpConStatus_t status = TCP_NO_ERROR;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int true_val = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));
    if (sockfd < 0) {
        logPrintf(ERROR_LOG, "ERROR opening socket");
        close(sockfd);
        return TCP_ERROR_OPENING_PORT;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    int n = 0;
    n = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    if (n < 0) {
        logPrintf(ERROR_LOG, "ERROR on binding\n");
        close(sockfd);
        return TCP_ERROR_ON_BINDING;
    }

    if (-1 == listen(sockfd, 5)) {
        logPrintf(ERROR_LOG, "ERROR on listen\n");
        close(sockfd);
        return TCP_ERROR_ON_LISTEN;
    }

    return status;
}

tcpConStatus_t tcpConnection::receive(char* rcv_buffer, int rcv_buffer_len, int& received) {
    tcpConStatus_t status = TCP_NO_ERROR;

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd >= 0)
    {
        bzero(rcv_buffer, rcv_buffer_len);
        received = read(newsockfd, rcv_buffer, rcv_buffer_len);
        if (received < 0) {
            logPrintf(ERROR_LOG, "ERROR reading from socket\n");
            status = TCP_ERROR_READ_FROM_SOCKET;
            close(newsockfd);
        }
    } else {
        logPrintf(ERROR_LOG, "ERROR on accept\n");
        status = TCP_ERROR_ACCEPT_SOCKET;
        close(newsockfd);
    }

    return status;
}

tcpConStatus_t tcpConnection::send(const char* send_buffer, int send_buffer_len, int& sent) {
    tcpConStatus_t status = TCP_NO_ERROR;
    sent = write(newsockfd, send_buffer, send_buffer_len);
    if (sent < 0) {
        logPrintf(ERROR_LOG, "ERROR writing to socket\n");
        status = TCP_ERROR_WRITE_TO_SOCKET;
    }

    close(newsockfd);
    return status;
}

void tcpConnection::disconnect() {
    shutdown(sockfd, 2);
    close(sockfd);
}
