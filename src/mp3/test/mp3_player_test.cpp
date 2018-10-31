#include "gtest/gtest.h"

#include <mp3/mp3_player.h>

TEST (Mp3PlayerTest, OepnWrongFile)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("wrong_file.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::FILE_NOT_FOUND, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayAndOepnWrongFile)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    status = mp3_player.Play("wrong_file.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::FILE_NOT_FOUND, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayPauseAndPlayWrongFile)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    status = mp3_player.Pause();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PAUSED, mp3_player.GetState());

    status = mp3_player.Play("wrong_file.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::FILE_NOT_FOUND, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayPauseAndResumeNonExistingFile)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    status = mp3_player.Pause();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PAUSED, mp3_player.GetState());

    // Rename file to simulate deletion file
    rename("123.mp3", "1234.mp3");

    status = mp3_player.Resume();
    EXPECT_EQ(mp3player::Status::FILE_NOT_FOUND, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());

    // Revert test file
    rename("1234.mp3", "123.mp3");
}

TEST (Mp3PlayerTest, PlayStopState)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    status = mp3_player.Stop();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayAndWaitForEndState)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, StopPauseResumeState)
{
    mp3player::Mp3Player mp3_player;
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());

    mp3player::Status status = mp3_player.Pause();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());

    status = mp3_player.Resume();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayResumeState)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());

    status = mp3_player.Resume();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayPauseResumeStopPlayState)
{
    mp3player::Mp3Player mp3_player;

    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    status = mp3_player.Pause();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    status = mp3_player.SetVolume(70);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PAUSED, mp3_player.GetState());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    status = mp3_player.Resume();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());
    EXPECT_EQ(70, mp3_player.GetVolume());
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    status = mp3_player.Stop();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());

    status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());
}

TEST (Mp3PlayerTest, ChangeVolume)
{
    mp3player::Mp3Player mp3_player;
    mp3player::Status status =  mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE , mp3_player.GetState());

    status = mp3_player.SetVolume(100);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(100, mp3_player.GetVolume());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    status = mp3_player.SetVolume(75);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(75, mp3_player.GetVolume());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    status = mp3_player.SetVolume(25);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(25, mp3_player.GetVolume());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    status = mp3_player.SetVolume(100);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(100, mp3_player.GetVolume());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    status = mp3_player.Stop();
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::STOPED, mp3_player.GetState());
}

TEST (Mp3PlayerTest, PlayAndDestructPlayer)
{
    EXPECT_NO_THROW
    ({
    mp3player::Mp3Player mp3_player;
    mp3player::Status status = mp3_player.Play("123.mp3", mp3player::PlayMode::ONCE);
    EXPECT_EQ(mp3player::Status::SUCCESS, status);
    EXPECT_EQ(mp3player::State::PLAY_ONCE, mp3_player.GetState());
    });
}

