#include "gtest/gtest.h"

#include <tcp_connection/tcp_client.h>
#include <tcp_connection/tcp_server.h>
#include <utilities/logger.h>
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
        ready_m = false;
    }

     virtual void TearDown()
    {
    }

    void ServerThread(int port, std::promise<std::string> && pr);
    void ClientThread(  std::string test_string
                      , std::string server_address
                      , int port
                      , std::promise<std::string> && pr);

    std::string ServerTask(int port);
    std::string ClientTask(  std::string test_string
                           , std::string server_address
                           , int port);

private:
    bool ready_m = false;
    std::mutex mu_m;
    std::condition_variable cv_m;
    utils::Logger logger_m;
};

void TcpConnection::ServerThread(int port, std::promise<std::string> && pr)
{
    char rcv_from_client[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Server tcp_server(port, logger_m);
    tcp_server.Connect();

    // Communicate client that server is ready.
    {
        std::lock_guard<std::mutex> lock(mu_m);
        ready_m = true;
    }
    cv_m.notify_one();

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
    std::unique_lock<std::mutex> lock(mu_m);
    while (!ready_m) cv_m.wait(lock);
    ready_m = false; // Clear ready state to be able to repeat test

    char rcv_from_server[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Client tcp_client(server_address, port, logger_m);
    tcp_client.Connect();
    tcp_client.Send(test_string.c_str(), test_string.length(), sent);
    tcp_client.Receive(rcv_from_server, tcp::BUFFER_SIZE, received);
    tcp_client.Disconnect();

    pr.set_value(std::string(rcv_from_server));
}

 
TEST_F (TcpConnection, SendReceiveThread) {
    int port = 1234;
    std::string server_address = "127.0.0.1"; // Use loopback address: 127.0.0.1, or 'localhost'
    std::string test_string = "Testing string ping-pong";

    for (int i = 0; i < 512 ; ++i) {
        std::promise<std::string> server_pr;
        std::future<std::string> server_future = server_pr.get_future();
        std::promise<std::string> client_pr;
        std::future<std::string> client_future = client_pr.get_future();

        std::thread server_thread (&TcpConnection::ServerThread, this, port, std::move(server_pr));
        std::thread client_thread (&TcpConnection::ClientThread, this, test_string, server_address, port, std::move(client_pr));

        server_future.wait();
        client_future.wait();

        server_thread.join();
        client_thread.join();

        std::string server_result = server_future.get();
        std::string client_result = client_future.get();

        //printf("Server value: %s\n", server_result.c_str());
        //printf("Client value: %s\n", client_result.c_str());

        EXPECT_STREQ(client_result.c_str(), server_result.c_str());
    }
}

std::string TcpConnection::ServerTask(int port)
{

    char rcv_from_client[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Server tcp_server(port, logger_m);
    tcp_server.Connect();

    // Communicate client that server is ready.
    {
        std::lock_guard<std::mutex> lock(mu_m);
        ready_m = true;
    }
    cv_m.notify_one();

    tcp_server.Receive(rcv_from_client, tcp::BUFFER_SIZE, received);
    tcp_server.Send(rcv_from_client, tcp::BUFFER_SIZE, sent);
    tcp_server.Disconnect();

    return std::string(rcv_from_client);
}

std::string TcpConnection::ClientTask(  std::string test_string
                                      , std::string server_address
                                      , int port)
{
    // Wait for server thread to start listening.
    std::unique_lock<std::mutex> lock(mu_m);
    while (!ready_m) cv_m.wait(lock);
    ready_m = false; // Clear ready state to be able to repeat test

    char rcv_from_server[tcp::BUFFER_SIZE] = {0};
    int sent = 0, received = 0;

    tcp::Client tcp_client(server_address, port, logger_m);
    tcp_client.Connect();
    tcp_client.Send(test_string.c_str(), test_string.length(), sent);
    tcp_client.Receive(rcv_from_server, tcp::BUFFER_SIZE, received);
    tcp_client.Disconnect();

    return std::string(rcv_from_server);
}


TEST_F (TcpConnection, SendReceiveTask) {
    int port = 1234;
    std::string server_address = "127.0.0.1"; // Use loopback address: 127.0.0.1, or 'localhost'
    std::string test_string = "Testing string ping-pong";

    for (int i = 0; i < 512 ; ++i) {
        auto server_task = std::async(std::launch::async, &TcpConnection::ServerTask, this, port);
        auto client_task = std::async(std::launch::async, &TcpConnection::ClientTask, this, test_string, server_address, port);

        std::string server_result = server_task.get();
        std::string client_result = client_task.get();

        EXPECT_STREQ(client_result.c_str(), server_result.c_str());
    }
}
