/*
 * app_thread_wrapper.hpp
 *
 *  Created on: Apr 12, 2016
 *      Author: mariusz
 */

#ifndef UTILITIES_APP_THREAD_WRAPPER_HPP_
#define UTILITIES_APP_THREAD_WRAPPER_HPP_

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

#endif /* UTILITIES_APP_THREAD_WRAPPER_HPP_ */
