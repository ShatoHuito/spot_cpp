//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_PACKETS_H
#define SPOT_CPP_PACKETS_H

#include <cstdint>
#include "../spot_class/spot.h"
#include "../packets/packet_struct.h"

class Spot;

class Packet{
public:
    static bool read_packet(packet_device_spot_t *packet, const uint8_t *buff);
    static bool read_packet_master_node(packet_device_spot_t *packet, const uint8_t *buff);
    static bool send_packet_to_master(packet_device_spot_t sub_packet, uint8_t device_rssi, uint16_t address, uint16_t master_fd);
    static bool handle_rx_packet(packet_device_spot_t *rcv_packet, Spot *spot);
    static bool handle_init_packet(packet_device_spot_t *rcv_packet, Spot *spot);
};


#endif //SPOT_CPP_PACKETS_H
