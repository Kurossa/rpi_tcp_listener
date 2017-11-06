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

using namespace tcp;

namespace
{

void CloseSocket(int& socket) {
    if (socket >= 0) {
        close(socket);
        socket = -1;
    }
}

} //namespace

ConStatus_t Connection::ChangeIp(const char* ip, const char* mask, const char* gateway) {
    struct ifreq ifr;
    int socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (-1 == socket_fd ) {
        return TCP_ERROR_CREATE_FD;
    }

    /* Set device's name */
    strncpy(ifr.ifr_name, interface_m.c_str(), IFNAMSIZ);

    /* Set IP */
    ifr.ifr_addr.sa_family = AF_INET;
    struct sockaddr_in* addr = (struct sockaddr_in*) &ifr.ifr_addr;
    inet_pton(AF_INET, ip, &addr->sin_addr);
    if (-1 == ioctl(socket_fd, SIOCSIFADDR, &ifr)) {
        CloseSocket(socket_fd);
        return TCP_ERROR_SET_IP;
    }

    /* Set netmask */
    addr = (struct sockaddr_in *) &(ifr.ifr_netmask);
    inet_pton(AF_INET, mask, &addr->sin_addr);
    if (-1 == ioctl(socket_fd, SIOCSIFNETMASK, &ifr)) {
        CloseSocket(socket_fd);
        return TCP_ERROR_SET_MASK;
    }

    //ioctl(fd, SIOCGIFFLAGS, &ifr);
    //strncpy(ifr.ifr_name, name, IFNAMSIZ);
    /* Set flags */
    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
    if (-1 == ioctl(socket_fd, SIOCSIFFLAGS, &ifr)) {
        CloseSocket(socket_fd);
        return TCP_ERROR_SET_FLAGS;
    }

    ConStatus_t status = SetRoute(socket_fd, gateway);

    CloseSocket(socket_fd);
    return status;
}

ConStatus_t Connection::SetRoute(int sockfd, const char *gateway_addr) {
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
        return TCP_ERROR_SET_ROUTE;
    }
    return TCP_NO_ERROR;
}

ConStatus_t Connection::Connect(void) {
    socket_fd_m = socket(AF_INET, SOCK_STREAM, 0);
    int true_val = 1;

    if (-1 == setsockopt(socket_fd_m, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int))
        || socket_fd_m < 0) {
        logPrintf(ERROR_LOG, "ERROR opening socket");
        CloseSocket(socket_fd_m);
        return TCP_ERROR_OPENING_PORT;
    }

    bzero((char *) &serv_addr_m, sizeof(serv_addr_m));
    serv_addr_m.sin_family = AF_INET;
    serv_addr_m.sin_addr.s_addr = INADDR_ANY;
    serv_addr_m.sin_port = htons(port_m);

    if (-1 == bind(socket_fd_m, (struct sockaddr *) &serv_addr_m, sizeof(serv_addr_m))) {
        logPrintf(ERROR_LOG, "ERROR on binding\n");
        CloseSocket(socket_fd_m);
        return TCP_ERROR_ON_BINDING;
    }

    if (-1 == listen(socket_fd_m, 5)) {
        logPrintf(ERROR_LOG, "ERROR on listen\n");
        CloseSocket(socket_fd_m);
        return TCP_ERROR_ON_LISTEN;
    }

    return TCP_NO_ERROR;
}

ConStatus_t Connection::Receive(char* rcv_buffer, int rcv_buffer_len, int& received) {
    client_m = sizeof(cli_addr_m);
    newsockfd_m = accept(socket_fd_m, (struct sockaddr *) &cli_addr_m, &client_m);
    if (newsockfd_m >= 0)
    {
        bzero(rcv_buffer, rcv_buffer_len);
        received = read(newsockfd_m, rcv_buffer, rcv_buffer_len);
        if (received < 0) {
            logPrintf(ERROR_LOG, "ERROR reading from socket\n");
            CloseSocket(newsockfd_m);
            return TCP_ERROR_READ_FROM_SOCKET;
        }
    } else {
        logPrintf(ERROR_LOG, "ERROR on accept\n");
        CloseSocket(newsockfd_m);
        return TCP_ERROR_ACCEPT_SOCKET;
    }

    return TCP_NO_ERROR;
}

ConStatus_t Connection::Send(const char* send_buffer, int send_buffer_len, int& sent) {
    if (newsockfd_m >= 0) {
        sent = write(newsockfd_m, send_buffer, send_buffer_len);
        if (sent < 0) {
            logPrintf(ERROR_LOG, "ERROR writing to socket\n");
            return TCP_ERROR_WRITE_TO_SOCKET;
        }

        CloseSocket(newsockfd_m);
        return TCP_NO_ERROR;
    } else {
        return TCP_ERROR_WRONG_SOCKET;
    }
}

void Connection::Disconnect() {
    CloseSocket(newsockfd_m);
    shutdown(socket_fd_m, 2);
    CloseSocket(socket_fd_m);
}
