//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_PACKETS_H
#define SPOT_CPP_PACKETS_H

#include "../defines.h"
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

class Packet{
public:
    static bool read_packet(packet_device_spot_t *packet, const uint8_t *buff);
    static bool read_packet_master_node(packet_device_spot_t *packet, const uint8_t *buff);
    static bool send_packet_to_master(packet_device_spot_t sub_packet, uint8_t device_rssi, uint16_t address, uint16_t master_fd);
};


#endif //SPOT_CPP_PACKETS_H
