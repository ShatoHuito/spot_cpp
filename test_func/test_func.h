//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_TEST_FUNC_H
#define SPOT_CPP_TEST_FUNC_H


#include <cstdint>
#include "../packets/packets.h"

bool print_small_paket(uint8_t *byte_arr);
bool print_packet(packet_device_spot_t pack);

#endif //SPOT_CPP_TEST_FUNC_H
