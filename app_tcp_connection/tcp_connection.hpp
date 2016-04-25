/*
 * tcp_connection.hpp
 *
 *  Created on: Apr 4, 2016
 *      Author: mariusz
 */

#ifndef APP_TCP_CONNECTION_TCP_CONNECTION_HPP_
#define APP_TCP_CONNECTION_TCP_CONNECTION_HPP_

#include <netinet/in.h>

#define TCP_BUFFER_SIZE (1024)

typedef enum tcpConStatus {
    TCP_ERROR_GENERAL = 0,
    TCP_ERROR_OPENING_PORT,
    TCP_ERROR_ON_BINDING,
    TCP_ERROR_ACCEPT_SOCKET,
    TCP_ERROR_WRITE_TO_SOCKET,
    TCP_ERROR_READ_FROM_SOCKET,
    TCP_STATUS_NUM
} tcpConStatus_t;

class tcpConnection {
public:
    tcpConnection(int port_no) :
            sockfd(), newsockfd(0), portno(port_no), clilen(0) {
    }
    ~tcpConnection(void) {
    }
    tcpConStatus_t connect(void);
    tcpConStatus_t receive(char* rcv_buffer, int rcv_buffer_len, int& received);
    tcpConStatus_t send(const char* send_buffer, int send_buffer_len, int& sent);
    void disconnect();
private:
    int sockfd;
    int newsockfd;
    int portno;
    unsigned int clilen;
    char buffer[TCP_BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
};

#endif /* APP_TCP_CONNECTION_TCP_CONNECTION_HPP_ */
