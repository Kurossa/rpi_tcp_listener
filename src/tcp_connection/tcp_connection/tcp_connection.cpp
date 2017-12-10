/*
 * tcp_connection.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: Kurossa
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

ConStatus_t SetRoute(int socket_fd, const std::string& gateway_addr) {
    struct rtentry route;
    memset(&route, 0, sizeof(route));
    struct sockaddr_in *addr = (struct sockaddr_in*) &route.rt_gateway;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(gateway_addr.c_str());
    addr = (struct sockaddr_in*) &route.rt_dst;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr = (struct sockaddr_in*) &route.rt_genmask;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_metric = 100;

    if (0 != ioctl(socket_fd, SIOCADDRT, &route)) {
        return TCP_ERROR_SET_ROUTE;
    }
    return TCP_NO_ERROR;
}

} //namespace

void tcp::CloseSocket(int& socket) {
    if (socket >= 0) {
        close(socket);
        socket = -1;
    }
}

ConStatus_t tcp::ChangeIp(  const std::string& interface
                         , const std::string& ip
                         , const std::string& mask
                         , const std::string& gateway) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    int socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

    if (-1 == socket_fd ) {
        return TCP_ERROR_CREATE_FD;
    }

    /* Set device's name */
    strncpy(ifr.ifr_name, interface.c_str(), IFNAMSIZ);

    /* Set IP */
    ifr.ifr_addr.sa_family = AF_INET;
    struct sockaddr_in* addr = (struct sockaddr_in*) &ifr.ifr_addr;
    inet_pton(AF_INET, ip.c_str(), &addr->sin_addr);
    if (-1 == ioctl(socket_fd, SIOCSIFADDR, &ifr)) {
        CloseSocket(socket_fd);
        return TCP_ERROR_SET_IP;
    }

    /* Set netmask */
    addr = (struct sockaddr_in *) &(ifr.ifr_netmask);
    inet_pton(AF_INET, mask.c_str(), &addr->sin_addr);
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

ConStatus_t Connection::CreateSocket(void) {
    conn_socket_fd_m = socket(AF_INET, SOCK_STREAM, 0);
    //conn_socket_fd_m = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.

    bzero((char *) &my_addr_m, sizeof(my_addr_m));
    my_addr_m.sin_family = AF_INET;
    my_addr_m.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1"); //INADDR_LOOPBACK; //INADDR_ANY;
    my_addr_m.sin_port = htons(port_m);

    //printf("Server-Using %s and port %d...\n", inet_ntoa(serv_addr_m.sin_addr), port_m);

    /* zero the rest of the struct */
    //memset(&(my_addr_m.sin_zero), '\0', 8);

    return TCP_NO_ERROR;
}

ConStatus_t Connection::ReceiveFromSocket(int socket_fd, char* rcv_buffer, int rcv_buffer_len, int& received) {
    if (socket_fd >= 0)
    {
        bzero(rcv_buffer, rcv_buffer_len);
        received = read(socket_fd, rcv_buffer, rcv_buffer_len);
        if (received < 0) {
            logPrintf(ERROR_LOG, "ERROR reading from socket\n");
            return TCP_ERROR_READ_FROM_SOCKET;
        }
    } else {
        logPrintf(ERROR_LOG, "ERROR no socket to receive from\n");
        return TCP_ERROR_WRONG_SOCKET;
    }

    return TCP_NO_ERROR;
}

ConStatus_t Connection::SendToSocket(int socket_fd, const char* send_buffer, int send_buffer_len, int& sent) {
    if (socket_fd >= 0) {
        sent = write(socket_fd, send_buffer, send_buffer_len);
        if (sent < 0) {
            logPrintf(ERROR_LOG, "ERROR writing to socket\n");
            return TCP_ERROR_WRITE_TO_SOCKET;
        }
        return TCP_NO_ERROR;
    } else {
        logPrintf(ERROR_LOG, "ERROR no socket to write\n");
        return TCP_ERROR_WRONG_SOCKET;
    }
}

void Connection::ShutdownSocket() {
    shutdown(conn_socket_fd_m, 2);
    CloseSocket(conn_socket_fd_m);
}

