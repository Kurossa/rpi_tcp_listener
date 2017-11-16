/*
 * tcp_connection.h
 *
 *  Created on: Apr 4, 2016
 *      Author: Kurossa
 */

#ifndef APP_TCP_CONNECTION_TCP_CONNECTION_H_
#define APP_TCP_CONNECTION_TCP_CONNECTION_H_

#include <netinet/in.h>
#include <string>

namespace tcp
{


const int BUFFER_SIZE = 1024;
const int MAX_CONNECTIONS = 10;

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
    TCP_ERROR_CONNECT_SOCKET,
    TCP_ERROR_SERVER_ADDRESS,
    TCP_ERROR_WRONG_SOCKET,
    TCP_ERROR_WRITE_TO_SOCKET,
    TCP_ERROR_READ_FROM_SOCKET,
    TCP_STATUS_NUM
} ConStatus_t;

void CloseSocket(int& socket);

ConStatus_t ChangeIp(  const std::string& interface
                     , const std::string& ip
                     , const std::string& mask
                     , const std::string& gateway);

class Connection {
public:
    Connection(const int port) : port_m(port) {}
    ~Connection(void) {}

    ConStatus_t CreateSocket(void);
    ConStatus_t ReceiveFromSocket(int socket_fd, char* rcv_buffer, int rcv_buffer_len, int& received);
    ConStatus_t SendToSocket(int socket_fd, const char* send_buffer, int send_buffer_len, int& sent);
    void ShutdownSocket();

protected:
    int conn_socket_fd_m = -1;
    struct sockaddr_in my_addr_m;
    const int port_m = 0;

};


} // namespace tcp

#endif /* APP_TCP_CONNECTION_TCP_CONNECTION_H_ */
