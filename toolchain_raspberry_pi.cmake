set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
# SDK location
set(SDK_PATH $ENV{HOME}/rpi_sdk)

# Specify the cross compiler
set(CMAKE_C_COMPILER ${SDK_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${SDK_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++)

# Set sysroot enviroment
set(CMAKE_FIND_ROOT_PATH ${SDK_PATH}/rootfs)
set(CMAKE_SYSROOT "${CMAKE_FIND_ROOT_PATH}")

# Search for programs only in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers only in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


