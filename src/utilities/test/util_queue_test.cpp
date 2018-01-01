#include "gtest/gtest.h"

#include <utilities/message_queue.h>

#include <cstdlib>

class QueueTest : public ::testing::Test
{
public:

protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


TEST_F (QueueTest, AddToQueueTest) {
    EXPECT_TRUE(true);
}
