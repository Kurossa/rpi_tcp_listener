/*
 * tcp_connection.hpp
 *
 *  Created on: Apr 4, 2016
 *      Author: mariusz
 */

#ifndef APP_TCP_CONNECTION_TCP_CONNECTION_HPP_
#define APP_TCP_CONNECTION_TCP_CONNECTION_HPP_

#include <netinet/in.h>
#include <string>

namespace tcp
{

const int BUFFER_SIZE = 1024;

typedef enum ConStatus {
    TCP_NO_ERROR = 0,
    TCP_ERROR_CREATE_FD,
    TCP_ERROR_SET_IP,
    TCP_ERROR_SET_MASK,
    TCP_ERROR_SET_ROUTE,
    TCP_ERROR_SET_FLAGS,
    TCP_ERROR_OPENING_PORT,
    TCP_ERROR_ON_BINDING,
    TCP_ERROR_ON_LISTEN,
    TCP_ERROR_ACCEPT_SOCKET,
    TCP_ERROR_WRONG_SOCKET,
    TCP_ERROR_WRITE_TO_SOCKET,
    TCP_ERROR_READ_FROM_SOCKET,
    TCP_STATUS_NUM
} ConStatus_t;

class Connection {
public:
    Connection(const std::string& interface,const int port) : interface_m(interface), port_m(port) {}
    ~Connection(void) {}
    ConStatus_t ChangeIp(const char* ip, const char* mask, const char* gateway);
    ConStatus_t SetRoute(int socket_fd_m, const char *gateway_addr);
    ConStatus_t Connect(void);
    ConStatus_t Receive(char* rcv_buffer, int rcv_buffer_len, int& received);
    ConStatus_t Send(const char* send_buffer, int send_buffer_len, int& sent);
    void Disconnect();

private:
    int socket_fd_m = -1;
    int newsockfd_m = -1;
    const std::string interface_m;
    const int port_m = 0;
    unsigned int client_m = 0;
    struct sockaddr_in serv_addr_m, cli_addr_m;
};

} // namespace tcp

#endif /* APP_TCP_CONNECTION_TCP_CONNECTION_HPP_ */
