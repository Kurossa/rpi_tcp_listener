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
#include <mutex>
#include <condition_variable>

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
    ~cMessageQueue() {}

    void enqueue(T& msg) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        msg_queue.push(msg);
        queue_cond_var.notify_one();
    }

    T dequeue()
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        while (msg_queue.empty())
        {
            queue_cond_var.wait(lock);
        }
        T ret_val = msg_queue.back();
        msg_queue.pop();

        return ret_val;
    }

    void push(T& msg) {
        std::lock_guard<std::mutex> lock(queue_mutex);
        msg_queue.push(msg);
        queue_cond_var.notify_one();
    }

    eMessageQueue_t pop(T& msg) {
        int status = MQ_NUM;
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (!msg_queue.empty()) {
            msg = msg_queue.back();
            msg_queue.pop();
            status = MQ_MSG_POP;
        } else {
            status = MQ_NO_MESSAGE;
        }
        return (eMessageQueue_t)status;
    }

    eMessageQueue_t poke(T& msg) {
        int status = MQ_NUM;
        std::lock_guard<std::mutex> lock(queue_mutex);
        if (!msg_queue.empty()) {
            msg = msg_queue.back();
            status = MQ_MSG_PENDING;
        } else {
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
    std::mutex queue_mutex;
    std::condition_variable queue_cond_var;
};

#endif /* UTILITIES_APP_MESSAGE_QUEUE_HPP_ */
