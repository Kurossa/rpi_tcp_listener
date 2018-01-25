
/*
 * app_thread_wrapper.hpp
 *
 *  Created on: Apr 12, 2016
 *      Author: Kurossa
 */

#ifndef UTILITIES_THREAD_WRAPPER_H_
#define UTILITIES_THREAD_WRAPPER_H_

#include <pthread.h>

class cPthreadWrapper {
public:
    virtual int startThread(void) = 0;
    virtual int stopThread(void) = 0;
    virtual void* action(void) = 0;

    cPthreadWrapper() : threadId_m(-1) {}
    virtual ~cPthreadWrapper() {}

protected:
    pthread_t threadId_m;

    static void* doAction(void* arg) {
        return static_cast<cPthreadWrapper*>(arg)->action();
    }
};

#endif /* UTILITIES_THREAD_WRAPPER_H_ */
