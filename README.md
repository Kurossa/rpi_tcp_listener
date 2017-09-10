# rpi_tcp_listener

# Compile
## Native compilation
`$ cmake -DCMAKE_BUILD_TYPE=<type> <path_to_project>`
`$ make`
type: Debug, Release
path_to_project: location of rpi_tcp_listener sources

## Cross-compilation
`$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$HOME/rpi_original/toolchain_raspberry_pi.cmake ../
`
`$ make`