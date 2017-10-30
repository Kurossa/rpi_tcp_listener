# RaspberryPi tcp server
This application implements tcp server. It implements simple plain text protocol to control mp3 player using RapsberryPi. It supports configuration from file, reconfiguration of network setings like: ip, mask or gateway.

# Requirements
This is linux based application. Requires pthread, mpg123 and ao external libs. It was compiled and tested on x86 linux and RaspberryPi 1,2 and 3.

# Builting target
## Native build
```
$ cmake -DCMAKE_BUILD_TYPE=<type> <path_to_project>
$ make
```
type: Debug, Release
path_to_project: location of rpi_tcp_listener sources

## Cross-compilation build
### Prepare RaspberryPi sdk
Get RaspberryPi toolchain

```
$ mkdir ~/rpi_sdk
$ cd ~/rpi_sdk
$ git clone https://github.com/raspberrypi/tools.git
```

Toolchain we will use, is located here:

```
tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/
```

Prepare rootfs based on target. This way we will have access to includes and libs from target.

```
$ cd ~/rpi_sdk
$ mkdir -p rootfs/opt
$ cp /<sd_card>/root/lib/ rootfs/ -rv
$ cp /<sd_card>/root/usr/ rootfs/ -rv
$ cp /<sd_card>/root/opt/vc rootfs/opt/ -rv
```

If there is no access to SD card, but there is RPi linux *.img file available, then it can be mounted. This can be source for rootfs libs and includes.

```
$ fdisk -l /<path_to_image>

Disk Stick.img: 3984 MB, 3984588800 bytes
249 heads, 6 sectors/track, 5209 cylinders, total 7782400 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x0004bfaa

    Device Boot      Start         End      Blocks   Id     System
Stick.img1   *         128     8015999     4007936    b  W95 FAT32
```

So the block-size is 512 bytes and the start-block is 128. The offset is 512 * 128 = 65536. Now image file can be mounted:

```
$ sudo mount -o loop,offset=<offset> <image_name>.img <target_folder>
```

#### Build

```
$ cd <project_folder>
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../toolchain_raspberry_pi.cmake ../
$ make
```

To build with unit tests add flag -DTEST=ON

# Run application on target
Example:

```
$ scp CMakeHelloWorld pi@192.168.1.PI:/home/pi/
$ ssh pi@192.168.1.PI ./CMakeHelloWorld
```
