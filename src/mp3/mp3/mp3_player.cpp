/*
 * app_mp3.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#include "mp3_player.h"
#include <utilities/logger.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>

#define MAX_INFINITE_LOOPS (100)
#define MP3_SAMPLE_SIZE (16384)
#define BITS (8)

using std::mutex;
using std::lock_guard;
using namespace mp3server;

void Mp3Player::SetState(PlayerState* new_state)
{
    //lock_guard<mutex> lock(state_mutex_m);
    state_m->OnLeave();
    state_m.reset(new_state);
    state_m->OnEnter();
}

Status Mp3Player::Play(std::string file_name, PlayMode play_mode)
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->Play(file_name, play_mode);
}

Status Mp3Player::Pause()
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->Pause();
}

Status Mp3Player::Stop()
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->Stop();
}

Status Mp3Player::SetVolume(uint16_t volume)
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->SetVolume(volume);
}

void Mp3Player::RunPlayThread(std::string& file_name, PlayMode play_mode)
{
    {
        lock_guard<mutex> lock(play_thread_mutex_m);
        play_thread_stop_m = false;
    }
    InitPlayer();
    play_thread_m = std::thread(&Mp3Player::DoPlay, this, file_name, play_mode);
}

void Mp3Player::StopPlayThread()
{
    {
        lock_guard<mutex> lock(play_thread_mutex_m);
        play_thread_stop_m = true;
    }
    if (play_thread_m.joinable()) {
        play_thread_m.join();
    }
    ResetPlayer();
}

void Mp3Player::DoPlay(std::string file_name, PlayMode play_mode)
{
    //TODO: Add implementation
    UNUSED_PARAM(file_name);
    UNUSED_PARAM(play_mode);
    int n = 50;
    bool stop_loop = false;
    do
    {
        printf("Playing loop n: %d\n", n);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        --n;
        {
            lock_guard<mutex> lock(play_thread_mutex_m);
            stop_loop = play_thread_stop_m;
        }
    } while (!stop_loop && n > 0);
}

void Mp3Player::InitPlayer()
{
    // Initialize mpg123 and ao
    ao_initialize();
    ao_driver_id_m = ao_default_driver_id();
    mpg123_init();
    mh_m = mpg123_new(NULL, &err_m);
    buffer_mpg_size_m = mpg123_outblock(mh_m);
    buffer_mpg_size_m = MP3_SAMPLE_SIZE;
    buffer_mpg_m = (unsigned char*) malloc(buffer_mpg_size_m * sizeof(unsigned char));

}
void Mp3Player::ResetPlayer()
{
    // Reset mpg123 and ao
    free(buffer_mpg_m);
    ao_close(ao_dev_m);
    mpg123_close(mh_m);
    mpg123_delete(mh_m);
    mpg123_exit();
    ao_shutdown();
    buffer_mpg_m = 0;
    ao_dev_m = 0;
    mh_m = 0;

    buffer_mpg_size_m = 0;
    buffer_mpg_done_m = 0;
    err_m = 0;
    ao_driver_id_m = 0;
}

