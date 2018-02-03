/*
 * main.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Kurossa
 */

#include <utilities/logger.h>
#include <utilities/zip.h>
#include <mp3/mp3.h>
#include <tcp_connection/tcp_server.h>
#include <communication/communication.h>
#include <config_manager/config_manager.h>
#include <signal.h>
#include <getopt.h>
#include <cstring>

using namespace utils;
using namespace communication;
using std::string;
using std::list;
using std::vector;

// Globals
int stop_proccess_g = 0;

// Main additional functions
static void SignalInterruptHandler(int signo) {
    if (signo == SIGINT) {
        logPrintf(LogLevel::SCREEN, "Interrupting application.\n");
        stop_proccess_g = 1;
    } else if (signo == SIGTERM) {
        logPrintf(LogLevel::SCREEN, "Terminating application.\n");
        stop_proccess_g = 1;
    }
}

void InitSignalInterruptHandler() {
    if (signal(SIGINT, SignalInterruptHandler) == SIG_ERR) {
        logPrintf(LogLevel::SCREEN, "Cannot handle SIGINT \n");
    }

    if (signal(SIGTERM, SignalInterruptHandler) == SIG_ERR) {
        logPrintf(LogLevel::SCREEN, "Cannot handle SIGTERM \n");
    }
}

// Usage print
void PrintUsage(char* appName) {
    printf("Usage: %s [-v] [-l] [-h]\n", appName);
    printf("\t-v Verbose mode\n");
    printf("\t-l Logging to log file\n");
    printf("\t-h Help\n");
}

// Parse main arguments
void ParseArgs(int argc, char** argv)
{
    int option = 0;
    while ((option = getopt(argc, argv, "vlh")) != -1) {
        switch (option) {
        case 'v':
            printf("Running in verbose mode\n");
            logSetVerboseMode(true);
            break;
        case 'l':
            printf("Logging to file\n");
            logSetLogToFile(true);
            break;
        case 'h':
            PrintUsage(argv[0]);
            exit(0);
            break;
        default:
            printf("Wrong usage!\n");
            PrintUsage(argv[0]);
            printf("ABORT!\n");
            exit(0);
        }
    }
}

// Main loop
int main(int argc, char** argv) {
    printf("Starting %s.\n", argv[0]);

    // Parse main arguments
    ParseArgs(argc, argv);

    // Flush stdout buffer immediately
    setbuf(stdout, NULL);

    // Initialize logger
    logInit();
    InitSignalInterruptHandler();
    logPrintf(LogLevel::SCREEN, "Starting application.\n");

    // Configuration Manager
    config::ConfigManager config_manager;
    if (config::OK == config_manager.ParseConfigFile(config_file_g)) {
        config::Configuration app_config = config_manager.GetConfig();
        logPrintf(LogLevel::SCREEN, "Configuration done.\n");

        // TCP Connection
        tcp::Server server = tcp::Server( app_config.port);
        tcp::ChangeIp(  std::string(network_interface_g)
                      , app_config.ip_str
                      , app_config.mask_str
                      , app_config.gateway_str);
        usleep(500000);
        server.Connect();
        logPrintf(LogLevel::SCREEN, "TCP server start.\n");

        // Communication
        char cmdMsg[tcp::BUFFER_SIZE];
        char replyMsg[tcp::BUFFER_SIZE];
        Communication communication(config_manager);

        // server loop
        while (0 == stop_proccess_g) {
            int recvdBytes, sentBytes;
            sprintf(replyMsg,"UNKNOWN_COMMAND\nEND\n");

            if (tcp::TCP_NO_ERROR == server.Receive(cmdMsg, tcp::BUFFER_SIZE, recvdBytes)) {
                communication.handleCommand(cmdMsg, replyMsg);
                server.Send(replyMsg, strlen(replyMsg), sentBytes);
            }

            //printf("mqSend size = %d, mqReply size = %d\n", mqSend.size(), mqReply.size());
            //printf("Received = %d, Sent = %d\n", recvdBytes, sentBytes);
        }
        logPrintf(LogLevel::SCREEN, "Sound server stop.\n");
        server.Disconnect();
        logPrintf(LogLevel::SCREEN, "TCP server stop.\n");
    } else {
        logPrintf(LogLevel::ERROR, "Configuration Failed.\n");
    }

    logPrintf(LogLevel::SCREEN, "End application.\n");
    logClose();
    return 0;
}
