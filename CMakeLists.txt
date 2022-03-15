cmake_minimum_required(VERSION 3.16)
project(spot_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(spot_cpp main.cpp spot_class/spot.cpp spot_class/spot.h device_class/device.h defines.h packets/packets.h main.h radio_receive/radio_receive.h network/socket_src.cpp network/socket_src.h test_func/test_func.cpp test_func/test_func.h crc/crc.cpp crc/crc.h network/connect_to_master.cpp network/connect_to_master.h message_exchange/message_exchange.cpp message_exchange/message_exchange.h)
