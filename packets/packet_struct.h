//
// Created by ilnur on 15.03.2022.
//

#ifndef SPOT_CPP_PACKET_STRUCT_H
#define SPOT_CPP_PACKET_STRUCT_H

#include <cstdint>

#define __PACKED__ __attribute__((__packed__))

typedef struct __PACKED__
{
    uint8_t		length;
    uint8_t		counter:2;
    uint16_t	address:14;
    uint32_t	info;
    uint16_t	crc;
} packet_device_spot_t;

typedef struct __PACKED__
{
    uint8_t length;
    //uint8_t counter:2;
    uint16_t address;
    int8_t rssi;
    packet_device_spot_t sub_packet;
    uint16_t crc;
} packet_spot_master_t;

#endif //SPOT_CPP_PACKET_STRUCT_H
