cmake_minimum_required(VERSION 3.16)
project(spot_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(spot_cpp main.cpp
        crc/crc.cpp
        crc/crc.h
        spot_class/spot.cpp
        spot_class/spot.h
        device_class/device.h
        defines.h
        packets/packets.h
        radio_receive/radio_receive.h
        network/network.cpp
        network/network.h
        test_func/test_func.cpp
        test_func/test_func.h
        packets/packets.cpp packets/packet_struct.h)
