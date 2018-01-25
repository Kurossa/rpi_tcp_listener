/*
 * tcp_server.h
 *
 *  Created on: Nov 14, 2017
 *      Author: Kurossa
 */

#ifndef TCP_CONNECTION_TCP_SERVER_H_
#define TCP_CONNECTION_TCP_SERVER_H_

#include "tcp_connection.h"

namespace tcp
{


class Server : private Connection {
public:
    Server(const int port) : Connection(port) {}
    ~Server(void) {}

    ConStatus_t Connect(void);
    void Disconnect(void);
    ConStatus_t Receive(char* rcv_buffer, int rcv_buffer_len, int& received);
    ConStatus_t Send(const char* send_buffer, int send_buffer_len, int& sent);
    int GetPort(void) const { return port_m; }

private:
    ConStatus_t BindAndListen(void);
    ConStatus_t Accept(void);

    int client_socket_fd_m = -1;
    struct sockaddr_in client_addr_m;
};


} // namespace tcp

#endif /* APP_TCP_CONNECTION_TCP_SERVER_H_ */
