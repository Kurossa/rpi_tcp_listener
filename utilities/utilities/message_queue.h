/*
 * app_message_queue.hpp
 *
 *  Created on: Apr 12, 2016
 *      Author: mariusz
 */

#ifndef UTILITIES_APP_MESSAGE_QUEUE_HPP_
#define UTILITIES_APP_MESSAGE_QUEUE_HPP_

#include <pthread.h>
#include <queue>
#include <string>
#include <stdint.h>
#include <unistd.h>

typedef enum eMessageQueue {
    MQ_NO_MESSAGE = 0,
    MQ_MSG_PENDING,
    MQ_MSG_POP,
    MQ_MSG_TMO,
    MQ_NUM
}eMessageQueue_t;

template <typename T>
class cMessageQueue {
public:
    cMessageQueue(void) {}
    ~cMessageQueue() { pthread_mutex_destroy(&mu_queue);}

    void init(void) {
        pthread_mutexattr_t mutexattr;
        // Set the mutex as a recursive mutex
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
        // create the mutex with the attributes set
        pthread_mutex_init(&mu_queue, &mutexattr);
        // After initializing the mutex, the thread attribute can be destroyed
        pthread_mutexattr_destroy(&mutexattr);
    }

    void push(T& msg) {
        pthread_mutex_lock(&mu_queue);
        msg_queue.push(msg);
        pthread_mutex_unlock(&mu_queue);
    }

    eMessageQueue_t pop(T& msg) {
        int status = MQ_NUM;
        pthread_mutex_lock(&mu_queue);
        if (!msg_queue.empty()) {
            msg = msg_queue.back();
            msg_queue.pop();
            pthread_mutex_unlock(&mu_queue);
            status = MQ_MSG_POP;
        } else {
            pthread_mutex_unlock(&mu_queue);
            status = MQ_NO_MESSAGE;
        }
        return (eMessageQueue_t)status;
    }

    eMessageQueue_t poke(T& msg) {
        int status = MQ_NUM;
        pthread_mutex_lock(&mu_queue);
        if (!msg_queue.empty()) {
            msg = msg_queue.back();
            pthread_mutex_unlock(&mu_queue);
            status = MQ_MSG_PENDING;
        } else {
            pthread_mutex_unlock(&mu_queue);
            status = MQ_NO_MESSAGE;
        }
        return (eMessageQueue_t)status;
    }

    /* Wait message with timeout: -1 wait infinite, 0 do not wait, N wait N * 100 [ms] */
    eMessageQueue_t wait(T& msg, int tmo_ms) {
        int status = MQ_NUM;
        /* If timeout is set to 0 return pop message */
        if (0 == tmo_ms){
            status = pop(msg);
        } else {
            do {
                /* If timeout reached return with timeout status */
                if(0 == tmo_ms) {
                    status = MQ_MSG_TMO;
                    break;
                } else if (tmo_ms > 0) {
                    --tmo_ms;
                }
                status = pop(msg);
                /* If msg is pop, do not wait return immediately */
                if (MQ_MSG_POP == status) {
                    break;
                }
                usleep(100000u);
            } while(MQ_NO_MESSAGE == status);
        }
        return (eMessageQueue_t)status;
    }

    int size(void) {
        return msg_queue.size();
    }
private:
    std::queue<T> msg_queue;
    pthread_mutex_t mu_queue;
};

#endif /* UTILITIES_APP_MESSAGE_QUEUE_HPP_ */
