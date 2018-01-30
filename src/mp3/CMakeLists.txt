project (mp3)

find_library(PTHREAD_LIB pthread)
find_library(AO_LIB ao)
find_library(MPG123_LIB mpg123)

add_library (mp3
	mp3/mp3.cpp
	mp3/mp3.h
	mp3/mp3_player.cpp
	mp3/mp3_player.h
	mp3/mp3_player_interface.h
	mp3/player_state_interface.h
	mp3/player_play_state.cpp
	mp3/player_play_state.h
	mp3/player_pause_state.cpp
	mp3/player_pause_state.h
	mp3/player_stop_state.cpp
	mp3/player_stop_state.h
)

target_include_directories (mp3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (mp3 PRIVATE "${PTHREAD_LIB}" "${MPG123_LIB}" "${AO_LIB}"
    PUBLIC utilities app_config)

