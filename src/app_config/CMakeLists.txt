project (app_config)

add_library (app_config INTERFACE)

target_include_directories (app_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources (app_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/app_config/app_constants.h)

