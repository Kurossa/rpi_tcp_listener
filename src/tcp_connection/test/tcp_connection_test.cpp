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
    }

     virtual void TearDown()
    {
        logClose();
    }

    void RunServer() { server_async_m = std::async(&TcpConnection::ServerThread, this); }
    void RunClient() { client_async_m = std::async(&TcpConnection::ClientThread, this); }
    std::string GetResultFromServer() { return server_async_m.get(); }
    std::string GetResultFromClient() { return client_async_m.get(); }


    std::string ServerThread();
    std::string ClientThread();
private:

    std::future<std::string> server_async_m;
    std::future<std::string> client_async_m;

    std::mutex m_mu;
    std::condition_variable m_cv;
    bool m_ready = false;
};

//bool TcpConnection::m_ready = false;

std::string TcpConnection::ServerThread()
{
    std::lock_guard<std::mutex> lock(m_mu);
    char rcv[256] = {0};
    int received = 0;
    int my_port = 1123;

    tcp::Server tcp_server(my_port);
    tcp_server.Connect();

    m_ready = true;
    m_cv.notify_one();// notify_one();
    m_mu.unlock();

    tcp_server.Receive(rcv, 256, received);
    printf("Server received: %s", rcv);
    tcp_server.Send(rcv, 256, received);
    tcp_server.Disconnect();
    return std::string(rcv);
}

std::string TcpConnection::ClientThread()
{
    std::unique_lock<std::mutex> lock(m_mu);
    while (!m_ready) m_cv.wait(lock);

    std::string send = "Dupa\n";
    char rcv2[256] = {0};
    int sent = 0, received = 0;
    std::string my_address = "10.0.2.15";
    int my_port = 1123;
    tcp::Client tcp_client(my_address, my_port);

    tcp_client.Connect();
    tcp_client.Send(send.c_str(), send.length(), sent);
    tcp_client.Receive(rcv2, 256, received);
    printf("Client received: %s", rcv2);
    tcp_client.Disconnect();
    return std::string(rcv2);
}

 
TEST_F (TcpConnection, SendReceive) {
    //RunClient();
     //std::this_thread::sleep_for(std::chrono::microseconds(200));
    //RunServer();

    //GetResultFromClient();
    //GetResultFromServer();
   //auto a1 =  std::async(&TcpConnection::ServerThread, this);
   //auto a2 =  std::async(&TcpConnection::ClientThread, this);
   //a1.get();
   //a2.get();

   std::thread first (&TcpConnection::ServerThread, this);
   std::thread second (&TcpConnection::ClientThread, this);


   first.join();
   second.join();
}

