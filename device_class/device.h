//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_DEVICE_H
#define SPOT_CPP_DEVICE_H

#include "../spot_class/spot.h"
#include "../packets/packets.h"
#include <cstdint>

#include "../defines.h"
#include "../packets/packet_struct.h"


enum flag_e
{
    ACK = 0,
    NEW,
    RENEW,
    SENT,
    RESENT
};

typedef struct
{
    packet_device_spot_t 	packet;
    uint32_t	counter;
    enum flag_e	flag;
} packet_to_send_t;

typedef struct
{
    uint32_t 	counter;
    uint16_t 	address;
    uint32_t	serialNumber;
    packet_to_send_t queue[QUEUE_SIZE];
} device_info;

#endif //SPOT_CPP_DEVICE_H
