/*
 * tcp_client.cpp
 *
 *  Created on: Apr 14, 2017
 *      Author: Kurossa
 */

#include "tcp_client.h"

#include <utilities/logger.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>


using namespace tcp;


ConStatus_t Client::Connect(void) {
    ConStatus_t status = Connection::CreateSocket();
    if (TCP_NO_ERROR == status) {
        return ConnectClient();
    }
    return status;
}

void Client::Disconnect(void) {
    Connection::ShutdownSocket();
}

ConStatus_t Client::ConnectClient(void) {
    if((my_addr_m.sin_addr.s_addr = inet_addr(server_address_m.c_str())) == (unsigned long)INADDR_NONE)
    {
        struct hostent *hostp = gethostbyname(server_address_m.c_str());
        if(hostp == (struct hostent *)NULL)
        {
            CloseSocket(conn_socket_fd_m);
            return TCP_ERROR_SERVER_ADDRESS;
        }
        memcpy(&my_addr_m.sin_addr, hostp->h_addr, sizeof(my_addr_m.sin_addr));
    }

    if((connect(conn_socket_fd_m, (struct sockaddr *)&my_addr_m, sizeof(my_addr_m))) < 0)
    {
        CloseSocket(conn_socket_fd_m);
        return TCP_ERROR_CONNECT_SOCKET;
    }

    return TCP_NO_ERROR;
}

ConStatus_t Client::Receive(char* rcv_buffer, int rcv_buffer_len, int& received) {
    //ConStatus_t status =
    //CloseSocket(conn_socket_fd_m);
    return Connection::ReceiveFromSocket(conn_socket_fd_m, rcv_buffer, rcv_buffer_len, received);
}

ConStatus_t Client::Send(const char* send_buffer, int send_buffer_len, int& sent) {
    return Connection::SendToSocket(conn_socket_fd_m, send_buffer, send_buffer_len, sent);
}

