#include "gtest/gtest.h"

#include <tcp_connection/tcp_client.h>
#include <tcp_connection/tcp_server.h>
#include <utilities/logger.h>
#include <chrono>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>


class TcpConnection : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        logSetVerboseMode(true);
        logInit();
        m_ready = false;
    }

     virtual void TearDown()
    {
        logClose();
    }

    void ServerThread(int port, std::promise<std::string> && pr);
    void ClientThread(  std::string test_string
                      , std::string server_address
                      , int port
                      , std::promise<std::string> && pr);

private:
    std::mutex m_mu;
    std::condition_variable m_cv;
    bool m_ready = false;
};

void TcpConnection::ServerThread(int port, std::promise<std::string> && pr)
{
    std::lock_guard<std::mutex> lock(m_mu);
    char rcv_from_client[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Server tcp_server(port);
    tcp_server.Connect();

    // Communicate client that server is ready.
    m_ready = true;
    m_cv.notify_one();
    m_mu.unlock();

    tcp_server.Receive(rcv_from_client, tcp::BUFFER_SIZE, received);
    tcp_server.Send(rcv_from_client, tcp::BUFFER_SIZE, sent);
    tcp_server.Disconnect();

    pr.set_value(std::string(rcv_from_client));
}

void TcpConnection::ClientThread(  std::string test_string
                                 , std::string server_address
                                 , int port
                                 , std::promise<std::string> && pr)
{
    // Wait for server thread to start listening.
    std::unique_lock<std::mutex> lock(m_mu);
    while (!m_ready) m_cv.wait(lock);

    char rcv_from_server[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Client tcp_client(server_address, port);
    tcp_client.Connect();
    tcp_client.Send(test_string.c_str(), test_string.length(), sent);
    tcp_client.Receive(rcv_from_server, tcp::BUFFER_SIZE, received);
    tcp_client.Disconnect();

    pr.set_value(std::string(rcv_from_server));
}

 
TEST_F (TcpConnection, SendReceive) {
    int port = 1234;
    std::string server_address = "127.0.0.1"; // Use loopback address: 127.0.0.1, or 'localhost'
    std::string test_string = "Testing string ping-pong";

    std::promise<std::string> server_pr;
    std::future<std::string> server_future = server_pr.get_future();
    std::promise<std::string> client_pr;
    std::future<std::string> client_future = client_pr.get_future();

    std::thread server_thread (&TcpConnection::ServerThread, this, port, std::move(server_pr));
    std::thread client_thread (&TcpConnection::ClientThread, this, test_string, server_address, port, std::move(client_pr));

    server_thread.join();
    client_thread.join();

    std::string server_result = server_future.get();
    std::string client_result = client_future.get();

    printf("Server value: %s\n", server_result.c_str());
    printf("Client value: %s\n", client_result.c_str());

    EXPECT_STREQ(client_result.c_str(), server_result.c_str());
}

