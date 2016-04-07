/*
 * main.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: mkrzysie
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include "app_tcp_connection/tcp_connection.hpp"
#include "app_zip/unzip.h"
#include "utilities/app_logger.hpp"
#include "app_config_parser/app_config_manager.hpp"

int STOP_PROCCESS = 0;

void error(const char *msg) {
    perror(msg);
    exit(0);
}

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

int main(int argc, char** argv) {
    logInit();
    init_signal_interrupt_handler();

    logPrintf(SCREEN_LOG, "Hello rpi!\n");

    /* Config Manager */
    cConfigManager configManager("config.txt");
    if (CONFIG_MANAGER_OK == configManager.init()) {
        logPrintf(SCREEN_LOG, "Conf_OK\n");
    }

    /* ZIP */
    /*HZIP hz = OpenZip("stuff.zip", "ps2016");
     ZIPENTRY ze;
     GetZipItem(hz, -1, &ze);
     int numitems = ze.index;
     for (int i = 0; i < numitems; i++) {
     GetZipItem(hz, i, &ze);
     UnzipItem(hz, i, ze.name);
     }
     CloseZip(hz);*/

    char buffer[256];
    tcpConnection tcp = tcpConnection(6969);

    tcp.connect();

    while (0 == STOP_PROCCESS) {
        tcp.receive(buffer, 256);
    }

    tcp.disconnect();
    logClose();

    return 0;
}

