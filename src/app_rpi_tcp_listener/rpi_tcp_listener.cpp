/*
 * main.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Kurossa
 */

#include <utilities/logger.h>
#include <utilities/zip.h>
#include <tcp_connection/tcp_server.h>
#include <communication/communication.h>
#include <config_manager/config_manager.h>
#include <csignal>
#include <getopt.h>
#include <cstring>
#include <cstdlib>

using namespace utils;
using namespace comm;
using std::string;
using std::list;
using std::vector;

// Globals
bool stop_proccess_g = false;
bool reboot_system_g = false;

// Main additional functions
static void SignalInterruptHandler(int signo) {
    if (signo == SIGINT) {
        printf("Interrupting application.\n");
        stop_proccess_g = true;
    } else if (signo == SIGTERM) {
        printf("Terminating application.\n");
        stop_proccess_g = true;
    }
}

// Reboot system
void RebootSystem()
{
    printf("Stop application and reboot.\n");
    stop_proccess_g = true;
    reboot_system_g = true;
}

void InitSignalInterruptHandler() {
    if (signal(SIGINT, SignalInterruptHandler) == SIG_ERR) {
        printf("Cannot handle SIGINT \n");
    }

    if (signal(SIGTERM, SignalInterruptHandler) == SIG_ERR) {
        printf("Cannot handle SIGTERM \n");
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
void ParseArgs(bool& log_to_file, bool& log_verbose, int argc, char** argv)
{
    int option = 0;
    while ((option = getopt(argc, argv, "vlh")) != -1) {
        switch (option) {
        case 'v':
            printf("Running in verbose mode\n");
            log_verbose = true;
            break;
        case 'l':
            printf("Logging to file\n");
            log_to_file = true;
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
    // Flush stdout buffer immediately
    setbuf(stdout, NULL);
    printf("Starting %s.\n", argv[0]);


    // Parse main arguments
    bool  log_to_file = false, log_verbose = false;
    ParseArgs(log_to_file, log_verbose, argc, argv);
    utils::Logger logger(log_to_file, log_verbose);

    // Innitialize signal handlers
    InitSignalInterruptHandler();
    logger.Log(LogLevel::SCREEN, "Starting application.\n");

    // Configuration Manager
    config::ConfigManager config_manager(logger);
    if (config::MISSING_FIELDS_IN_CONFIG >= config_manager.ParseConfigFile(config_file_g)) {
        config::Configuration app_config = config_manager.GetConfig();
        logger.Log(LogLevel::SCREEN, "Configuration done.\n");

        // Change network ip/mask/gateway if other than in confige file
        tcp::ChangeIp(  std::string(network_interface_g)
                      , app_config.ip_str
                      , app_config.mask_str
                      , app_config.gateway_str);

        // TCP Connection
        tcp::Server server = tcp::Server(app_config.port, logger);
        server.Connect();
        logger.Log(LogLevel::SCREEN, "TCP server started.\n");

        // Communication
        char cmdMsg[tcp::BUFFER_SIZE];
        char replyMsg[tcp::BUFFER_SIZE];
        Communication communication(config_manager, logger, &RebootSystem);

        // server loop
        while (!stop_proccess_g) {
            int recvdBytes, sentBytes;
            sprintf(replyMsg,"UNKNOWN_COMMAND\nEND\n");

            if (tcp::TCP_NO_ERROR == server.Receive(cmdMsg, tcp::BUFFER_SIZE, recvdBytes)) {
                communication.handleCommand(cmdMsg, replyMsg);
                server.Send(replyMsg, strlen(replyMsg), sentBytes);
            }
            //printf("Received = %d, Sent = %d\n", recvdBytes, sentBytes);
        }
        server.Disconnect();
        logger.Log(LogLevel::SCREEN, "TCP server stop.\n");
    } else {
        logger.Log(LogLevel::ERROR, "Configuration Failed.\n");
    }

    logger.Log(LogLevel::SCREEN, "End application.\n");
    if(reboot_system_g) {
        logger.Log(LogLevel::SCREEN, "Rebooting system.\n");
        system("reboot");
    }
    return 0;
}
