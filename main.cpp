/*
 * main.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: mkrzysie
 */
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <app_logger.hpp>
#include <app_zip.hpp>
#include <app_mp3.hpp>
#include <tcp_connection.hpp>
#include <app_communication.hpp>
#include <app_config_manager.hpp>

using namespace std;

/* Globals */
int STOP_PROCCESS = 0;

/* Main additional functions */
static void signal_interrupt_handler(int signo) {
    if (signo == SIGINT) {
        logPrintf(SCREEN_LOG, "Interrupting application.\n");
        STOP_PROCCESS = 1;
    } else if (signo == SIGTERM) {
        logPrintf(SCREEN_LOG, "Terminating application.\n");
        STOP_PROCCESS = 1;
    }
}

void init_signal_interrupt_handler() {
    if (signal(SIGINT, signal_interrupt_handler) == SIG_ERR) {
        logPrintf(SCREEN_LOG, "Cannot handle SIGINT \n");
    }

    if (signal(SIGTERM, signal_interrupt_handler) == SIG_ERR) {
        logPrintf(SCREEN_LOG, "Cannot handle SIGTERM \n");
    }
}

/* Usage print */
void print_usage(char* appName) {
    printf("Usage: %s [-v] [-l] [-h]\n",appName);
    printf("\t-v Verbose mode\n");
    printf("\t-l Logging to log file\n");
    printf("\t-h Help\n");
}

/* Main loop */
int main(int argc, char** argv) {
    printf("Starting %s.\n", argv[0]);
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
            print_usage(argv[0]);
            return 0;
            break;
        default:
            printf("Wrong usage!\n");
            print_usage(argv[0]);
            printf("ABORT!\n");
            abort();
        }
    }

    /* Flush stdout buffer immediately */
    setbuf(stdout, NULL);

    /* Initialize logger */
    logInit();
    init_signal_interrupt_handler();
    logPrintf(SCREEN_LOG, "Starting application.\n");

    /* Configuration Manager */
    cConfigManager configManager(config_file);
    if (CONFIG_MANAGER_OK == configManager.init()) {

        int errors_num = 0;
        list<sDeviceConfig>* devicesList = configManager.get_devicesConfigList();
        sDeviceConfig devConfig = devicesList->front();

        /* only for testing */
        //zip_uncompress("file1_orig.mp3", "file1.mp3");
        //zip_uncompress("file2_orig.mp3", "file2.mp3");

        /* ZIP */
        vector<string>::iterator encSound_it = devConfig.sounds.begin();
        vector<string>::iterator ramSound_it = devConfig.soundsInRam.begin();
        for (; encSound_it < devConfig.sounds.end(); ++encSound_it, ++ramSound_it) {
            int status = zip_uncompress((*encSound_it).c_str(), (*ramSound_it).c_str());
            if (ZIP_OK != status) {
                ++errors_num;
            }
            printf("unzip %s to %s, with status %d\n", (*encSound_it).c_str(), (*ramSound_it).c_str(), status);
        }
        logPrintf(SCREEN_LOG, "Configuration done.\n");

        /* Initialize message queue(s) */
        mqSend_t mqSend;
        mqSend.init();
        mqReply_t mqReply;
        mqReply.init();

        /* Initialize mp3 server thread */
        cMp3Server mp3_server(&mqSend, &mqReply, devConfig.soundsInRam);
        mp3_server.startThread();
        logPrintf(SCREEN_LOG, "Sound server start.\n");

        /* TCP Connection */
        tcpConnection tcp = tcpConnection(devConfig.port);
        tcp.changeIp(devConfig.ipString.c_str(), devConfig.maskString.c_str());
        usleep(500000);
        tcp.connect();
        logPrintf(SCREEN_LOG, "TCP server start.\n");

        /* Communication */
        char cmdMsg[TCP_BUFFER_SIZE];
        char replyMsg[TCP_BUFFER_SIZE];
        cCommunication communication(&mqSend, &mqReply, &configManager);

        /* main loop */
        while (0 == STOP_PROCCESS && !errors_num) {
            int recvdBytes, sentBytes;
            sprintf(replyMsg,"UNKNOWN_COMMAND\nEND\n");

            tcp.receive(cmdMsg, TCP_BUFFER_SIZE, recvdBytes);
            communication.handleCommand(cmdMsg, replyMsg);
            tcp.send(replyMsg,strlen(replyMsg),sentBytes);

            printf("mqSend size = %d, mqReply size = %d\n", mqSend.size(), mqReply.size());
            printf("Received = %d, Sent = %d\n", recvdBytes, sentBytes);
        }
        mp3_server.stopThread();
        logPrintf(SCREEN_LOG, "Sound server stop.\n");
        tcp.disconnect();
        logPrintf(SCREEN_LOG, "TCP server stop.\n");
    } else {
        logPrintf(ERROR_LOG, "Configuration Failed.\n");
    }

    logPrintf(SCREEN_LOG, "End application.\n");
    logClose();
    return 0;
}
