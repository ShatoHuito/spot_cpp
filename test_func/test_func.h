//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_TEST_FUNC_H
#define SPOT_CPP_TEST_FUNC_H

#include "../packets/packets.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <csignal>
#include <stdint-gcc.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

bool print_small_paket(uint8_t *byte_arr);
bool print_packet(packet_device_spot_t pack);

#endif //SPOT_CPP_TEST_FUNC_H
