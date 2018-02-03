/*
 * app_mp3.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: Kurossa
 */

#include "mp3_player.h"
#include "player_stop_state.h"
#include <utilities/logger.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>

#define MAX_INFINITE_LOOPS (100)
#define MP3_SAMPLE_SIZE (16384)
#define BITS (8)

using std::mutex;
using std::lock_guard;
using namespace mp3player;

Mp3Player::Mp3Player():
    mh_m(nullptr),
    buffer_mpg_m(0),
    buffer_mpg_size_m(0),
    buffer_mpg_done_m(0),
    ao_driver_id_m(0),
    ao_dev_m(nullptr),
    volume_m(100),
    stop_thread_m(false),
    thread_is_running_m(false),
    cv_ready_m(false),
    stop_auto_stop_thread_m(false),
    thread_id_to_stop_m(std::thread::id())

{
    state_m.reset(new PlayerStopState(*this));
    auto_stop_thread_m = std::thread(&Mp3Player::DoStop, this);
}
Mp3Player::~Mp3Player()
{
    Stop();
    {
        std::lock_guard<std::mutex> lock(cv_mutex_m);
        thread_id_to_stop_m = std::thread::id();
        cv_ready_m = true;
        stop_auto_stop_thread_m = true;
    }
    cv_m.notify_one();
    auto_stop_thread_m.join();
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

Status Mp3Player::Resume()
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->Resume();
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

State Mp3Player::GetPlayMode()
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->GetState();
}

std::string Mp3Player::GetPlayModeStr()
{
    lock_guard<mutex> lock(state_mutex_m);
    return state_m->GetStateStr();
}

void Mp3Player::SetState(PlayerState* new_state)
{
    //lock_guard<mutex> lock(state_mutex_m);
    state_m->OnLeave();
    state_m.reset(new_state);
    state_m->OnEnter();
}

bool Mp3Player::OpenPlayer(std::string& file_name)
{
    // Initialize mpg123
    int error = 0;
    mpg123_init();
    mh_m = mpg123_new(NULL, &error);
    //buffer_mpg_size_m = mpg123_outblock(mh_m);
    buffer_mpg_size_m = MP3_SAMPLE_SIZE;
    buffer_mpg_m = (unsigned char*) malloc(buffer_mpg_size_m * sizeof(unsigned char));

    // Init mp3 file
    int channels = 0, encoding = 0;
    long rate = 0;

    //TODO: Add chcecking if file exists or can be open by mpg123
    int status = mpg123_open(mh_m, file_name.c_str());
    printf("Opening file status: %d\n",status);
    mpg123_getformat(mh_m, &rate, &channels, &encoding);

    // Initialize ao_driver
    printf("ao_initialize (count: %d)\n",ao_count_m);
    ao_count_m++;
    ao_initialize();
    ao_driver_id_m = ao_default_driver_id();
    // Set the output format
    ao_sample_format format;
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = static_cast<int>(rate);
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    ao_dev_m = ao_open_live(ao_driver_id_m, &format, NULL);

    //Set volume
    float volume_float = volume_m / 100.0;
    mpg123_volume(mh_m, volume_float);

    //FIXME: return real value
    return true;
}

void Mp3Player::ClosePlayer()
{
    // Reset mpg123 and ao
    free(buffer_mpg_m);
    mpg123_close(mh_m);
    mpg123_delete(mh_m);
    mpg123_exit();
    buffer_mpg_m = 0;
    buffer_mpg_size_m = 0;
    buffer_mpg_done_m = 0;
    mh_m = nullptr;

    ao_close(ao_dev_m);
    ao_shutdown();
    ao_dev_m = nullptr;
    ao_driver_id_m = 0;
}

void Mp3Player::RunPlayThread(std::string& file_name, PlayMode play_mode)
{
    lock_guard<std::mutex> lock_thread(thread_mutex_m);
    if (!thread_is_running_m)
    {
        {
            lock_guard<mutex> lock(stop_thread_mutex_m);
            stop_thread_m = false;
        }
        play_thread_m = std::thread(&Mp3Player::DoPlay, this, file_name, play_mode);
        thread_is_running_m = true;
    }
}

void Mp3Player::StopPlayThread()
{
    lock_guard<std::mutex> lock_thread(thread_mutex_m);
    if (thread_is_running_m)
    {
        {
            lock_guard<mutex> lock(stop_thread_mutex_m);
            stop_thread_m = true;
        }
        if (play_thread_m.joinable()) {
            play_thread_m.join();
            thread_is_running_m = false;
        }
    }
}

void Mp3Player::DoPlay(std::string file_name, PlayMode play_mode)
{
    int n = 0;
    bool stop_loop = false;
    uint16_t file_play_max_times = 0;
    uint16_t file_played_n_times = 0;

    switch (play_mode) {
    case PlayMode::ONCE:
        file_play_max_times = 1;
        break;
    case PlayMode::IN_LOOP:
        file_play_max_times = MAX_INFINITE_LOOPS;
        break;
    default:
        stop_loop = true;
        file_play_max_times = 0;
        printf("Wrong play mode: %d\n", play_mode);
        break;
    }

    while(!stop_loop && file_played_n_times < file_play_max_times)
    {
        while (!stop_loop && MPG123_OK == mpg123_read(mh_m, buffer_mpg_m, buffer_mpg_size_m, &buffer_mpg_done_m))
        {
            printf("Playing loop n: %d\n", n);
            ao_play(ao_dev_m, (char*)(buffer_mpg_m), buffer_mpg_done_m);
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            ++n;
            {
                lock_guard<mutex> lock(stop_thread_mutex_m);
                stop_loop = stop_thread_m;
            }
        }
        n = 0;

        ++file_played_n_times;
        // Check if this is last time file played, if yes, do not reopen player.
        if(!stop_loop && file_played_n_times < file_play_max_times)
        {
            ClosePlayer();
            OpenPlayer(file_name);
        }
    }

    // When file finished by its self
    if(!stop_loop) {
        std::lock_guard<std::mutex> lock(cv_mutex_m);
        thread_id_to_stop_m = std::this_thread::get_id();
        cv_ready_m = true;
        cv_m.notify_one();
    }
}

void Mp3Player::DoStop()
{
    do
    {
        std::unique_lock<std::mutex> lock_cv(cv_mutex_m);
        while (!cv_ready_m) cv_m.wait(lock_cv);
        cv_ready_m = false;

        lock_guard<mutex> lock(state_mutex_m);
        std::thread::id current_thread_id;
        {
            lock_guard<std::mutex> lock_thread(thread_mutex_m);
            current_thread_id = play_thread_m.get_id();
        }

        if (thread_id_to_stop_m != std::thread::id()
            && thread_id_to_stop_m == current_thread_id)
        {
            state_m->Stop();
            thread_id_to_stop_m = std::thread::id();
        }
    } while(!stop_auto_stop_thread_m);
}

