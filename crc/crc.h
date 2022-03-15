//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_CRC_H
#define SPOT_CPP_CRC_H

#include <cstdint>
#include "../packets/packets.h"

uint16_t dallasCrc16(uint8_t *data, uint8_t size);

/*
  16bit CRC notes:
  "CRC-CCITT"
    poly is g(X) = X^16 + X^12 + X^5 + 1  (0x1021)
    used in the FPGA (green boards and 15.4)
    initial remainder should be 0xFFFF
*/

uint16_t ccittCrc16(uint8_t *data, uint8_t size);
uint8_t check_CRC(packet_device_spot_t *packet);

#endif //SPOT_CPP_CRC_H
