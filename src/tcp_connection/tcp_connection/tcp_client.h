/*
 * tcp_client.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Kurossa
 */

#ifndef APP_TCP_CONNECTION_TCP_CLIENT_H_
#define APP_TCP_CONNECTION_TCP_CLIENT_H_

#include "tcp_connection.h"

namespace tcp
{


class Client : private Connection {
public:
    Client(std::string& address, const int port) : Connection(port), server_address_m(address) {}
    ~Client(void) {}

    ConStatus_t Connect(void);
    void Disconnect(void);
    ConStatus_t Receive(char* rcv_buffer, int rcv_buffer_len, int& received);
    ConStatus_t Send(const char* send_buffer, int send_buffer_len, int& sent);
    std::string GetAddress(void) const { return server_address_m; }
    int GetPort(void) const { return port_m; }

private:
    ConStatus_t ConnectClient(void);
    std::string server_address_m;
};


} // namespace tcp

#endif /* APP_TCP_CONNECTION_TCP_CLIENT_H_ */
