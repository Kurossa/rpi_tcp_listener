# RaspberryPi tcp server
This application implements tcp server. It implements simple plain text protocol to control mp3 player using RapsberryPi. It supports configuration from file, reconfiguration of network setings like: ip, mask or gateway.

# Requirements
This is linux based application. Requires pthread, mpg123 and ao external libs. It was compiled and tested on x86 linux and RaspberryPi 1,2 and 3.

# Builting target
## Native build
`$ cmake -DCMAKE_BUILD_TYPE=<type> <path_to_project>`
`$ make`
type: Debug, Release
path_to_project: location of rpi_tcp_listener sources

## Cross-compilation build
### Prepare RaspberryPi sdk
Get RaspberryPi toolchain
`$ mkdir ~/rpi_sdk`
`$ cd ~/rpi_sdk`
`$ git clone https://github.com/raspberrypi/tools.git`
Toolchain we will use is located here:
`tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/`
Prepare rootfs based on target, this way we will have access to includes and libs from target.
`$ cd ~/rpi_sdk`
`$ mkdir rootfs`
`$ make`
`$ make`

`$ make`
`$ make`

#### Build
`$ cd <project_folder>`
`$ mkdir build`
`$ cd build`
`$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../toolchain_raspberry_pi.cmake ../`
`$ make`