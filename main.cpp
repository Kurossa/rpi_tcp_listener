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
char ram[] = "/tmp/";
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

    /* Initialize message queue(s) */
    mqSend_t mqSend;
    mqSend.init();
    mqReply_t mqReply;
    mqReply.init();

    /* Initialize mp3 server thread */
    cMp3Server mp3_server(&mqSend, &mqReply);
    mp3_server.startThread();

    /* Config Manager */
    cConfigManager configManager("config.txt");
    if (CONFIG_MANAGER_OK == configManager.init()) {

        int errors_num = 0;
        list<sDeviceConfig>* devicesList = configManager.get_devicesConfigList();
        sDeviceConfig devConfig = devicesList->front();

        /* only for testing */
        //zip_uncompress("file1_orig.mp3", "file1.mp3");
        //zip_uncompress("file2_orig.mp3", "file2.mp3");

        /* ZIP */
        vector<string>::iterator it = devConfig.sounds.begin();
        for (; it < devConfig.sounds.end(); ++it) {
            char mp3Dest[256];
            bzero(mp3Dest,256);
            char mp3Source[(*it).length() + 1];
            strcpy(mp3Source, (*it).c_str());
            sprintf(mp3Dest,"%s%s",ram, mp3Source);
            int status = zip_uncompress(mp3Source, mp3Dest);
            if (ZIP_OK != status) {
                ++errors_num;
            }
            printf("unzip %s to %s, with status %d\n", mp3Source, mp3Dest, status);
        }
        logPrintf(SCREEN_LOG, "Configuration done.\n");

        /* TCP Connection */
        tcpConnection tcp = tcpConnection(devConfig.port);
        tcp.connect();

        /* Communication */
        char cmdMsg[TCP_BUFFER_SIZE];
        char replyMsg[TCP_BUFFER_SIZE];
        cCommunication communication(&mqSend, &mqReply);

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
        tcp.disconnect();
    } else {
        logPrintf(ERROR_LOG, "Configuration Failed.\n");
    }

    mp3_server.stopThread();

    logPrintf(SCREEN_LOG, "End application.\n");
    logClose();
    return 0;
}

