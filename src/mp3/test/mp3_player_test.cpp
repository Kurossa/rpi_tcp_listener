#include "gtest/gtest.h"

#include <mp3/mp3_player.h>

TEST (Mp3PlayerTest, DISABLED_PlayStopState) {

    mp3server::Mp3Player mp3_player;
    mp3_player.Play("test.mp3", mp3server::PlayMode::ONCE);
    mp3_player.SetVolume(100);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    mp3_player.SetVolume(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    mp3_player.SetVolume(100);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    mp3_player.SetVolume(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    mp3_player.Stop();
    mp3_player.Stop();

    printf("End of test!\n");
    EXPECT_TRUE(true);
}

TEST (Mp3PlayerTest, PlayPausePlayStopState) {

    mp3server::Mp3Player mp3_player;
    mp3_player.Play("test.mp3", mp3server::PlayMode::ONCE);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    mp3_player.Pause();
    mp3_player.SetVolume(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //FIXME: Repair Resume stat, probably DoPlay read again file.
    mp3_player.Resume();
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    mp3_player.Stop();

    printf("End of test!\n");
    EXPECT_TRUE(true);
}

TEST (Mp3PlayerTest, DISABLED_PlayStopStopState) {

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

TEST (Mp3PlayerTest, DISABLED_PlayAndDestructPlayer) {

    {
    mp3server::Mp3Player mp3_player;
    mp3_player.Play("1.mp3", mp3server::PlayMode::ONCE);
    }
    printf("End of test!\n");
    EXPECT_TRUE(true);
}
TEST (Mp3PlayerTest, DISABLED_PlayAndWaitForEnd) {
    mp3server::Mp3Player mp3_player;
    mp3_player.Play("1.mp3", mp3server::PlayMode::ONCE);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    printf("End of test!\n");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    mp3_player.Stop();
    EXPECT_TRUE(true);
}
