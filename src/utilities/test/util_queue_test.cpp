#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <utilities/message_queue.h>
#include <functional>
#include <future>
#include <vector>

namespace
{

std::vector<int> EnqueueTask(utils::MessageQueue<int>& mq, size_t size)
{
    std::vector<int> ret;
    for(int i = 0; i < (int)size; ++i)
    {
        int rnd_val = rand();
        mq.enqueue(rnd_val);
        ret.push_back(rnd_val);
    }
    return ret;
}

std::vector<int> DequeueTask(utils::MessageQueue<int>& mq, size_t size)
{
    std::vector<int> ret;
    for(int i = 0; i < (int)size; ++i)
    {
        ret.push_back(mq.dequeue());
    }
    return ret;
}

}

TEST (UtilitiesTest, EnqueueDequeueRandomNumbers) {
    utils::MessageQueue<int> mq;
    const size_t test_queue_size = 128;
    for (int i = 0; i < 512 ; ++i) {
        auto enqueue_task = std::async(std::launch::async, &EnqueueTask, std::ref(mq), test_queue_size);
        auto dequeue_task = std::async(std::launch::async, &DequeueTask, std::ref(mq), test_queue_size);

        auto input_vector = enqueue_task.get();
        auto output_vector = dequeue_task.get();

        EXPECT_THAT(input_vector, ::testing::ContainerEq(output_vector));
    }
}
