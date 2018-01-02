#include "gtest/gtest.h"

#include <mp3/mp3_player.h>

TEST (Mp3PlayerTest, PlayStopState) {

    mp3server::Mp3Player mp3_player;
    mp3_player.Play("1.mp3", mp3server::PlayMode::ONCE);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mp3_player.Play("2.mp3", mp3server::PlayMode::ONCE);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mp3_player.Stop();
    mp3_player.Stop();

    printf("End of test!\n");
    EXPECT_TRUE(true);
}

TEST (Mp3PlayerTest, PlayAndDestructPlayer) {

    {
    mp3server::Mp3Player mp3_player;
    mp3_player.Play("1.mp3", mp3server::PlayMode::ONCE);
    }
    printf("End of test!\n");
    EXPECT_TRUE(true);
}
