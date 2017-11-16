/*
 * tcp_server.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: Kurossa
 */

#include "tcp_server.h"

#include <utilities/logger.h>

using namespace tcp;


ConStatus_t Server::Connect(void) {
    ConStatus_t status = Connection::CreateSocket();
    if (TCP_NO_ERROR == status) {
        return BindAndListen();
    }
    return status;
}

void Server::Disconnect(void) {
    CloseSocket(client_socket_fd_m);
    Connection::ShutdownSocket();
}

ConStatus_t Server::Receive(char* rcv_buffer, int rcv_buffer_len, int& received) {
    ConStatus_t status = Accept();
    if (TCP_NO_ERROR == status) {

        status = Connection::ReceiveFromSocket(client_socket_fd_m, rcv_buffer, rcv_buffer_len, received);
        if (status != TCP_NO_ERROR) {
            CloseSocket(client_socket_fd_m);
        }
    }
    return status;
}

ConStatus_t Server::Send(const char* send_buffer, int send_buffer_len, int& sent) {
    ConStatus_t status = TCP_ERROR_WRONG_SOCKET;
    if (client_socket_fd_m >= 0) {
        status = Connection::SendToSocket(client_socket_fd_m, send_buffer, send_buffer_len, sent);
        CloseSocket(client_socket_fd_m);
    }
    return status;
}

ConStatus_t Server::BindAndListen(void) {
    int true_val = 1;

    if (-1 == setsockopt(conn_socket_fd_m, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int))
        || conn_socket_fd_m < 0) {
        logPrintf(ERROR_LOG, "ERROR opening socket");
        CloseSocket(conn_socket_fd_m);
        return TCP_ERROR_OPENING_PORT;
    }

    if (-1 == bind(conn_socket_fd_m, (sockaddr *) &my_addr_m, sizeof(my_addr_m))) {
        logPrintf(ERROR_LOG, "ERROR on binding\n");
        tcp::CloseSocket(conn_socket_fd_m);
        return TCP_ERROR_ON_BINDING;
    }

    if (-1 == listen(conn_socket_fd_m, tcp::MAX_CONNECTIONS)) {
        logPrintf(ERROR_LOG, "ERROR on listen\n");
        tcp::CloseSocket(conn_socket_fd_m);
        return TCP_ERROR_ON_LISTEN;
    }
    return TCP_NO_ERROR;
}

ConStatus_t Server::Accept(void)
{
    socklen_t addr_len = sizeof(client_addr_m);
    if (-1 == (client_socket_fd_m = accept(conn_socket_fd_m, (sockaddr *) &client_addr_m, &addr_len)))
    {
        logPrintf(ERROR_LOG, "ERROR on accept\n");
        // ?? Do we need that // ::CloseSocket(newsockfd_m);
        return TCP_ERROR_ACCEPT_SOCKET;
    }

    return TCP_NO_ERROR;
}

