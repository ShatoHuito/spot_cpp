//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_RADIO_RECEIVE_H
#define SPOT_CPP_RADIO_RECEIVE_H
#define __PACKED__ __attribute__((__packed__))

#include "../defines.h"

typedef struct {
    uint8_t est1;
    uint8_t est0;
}__attribute__((__packed__)) freqoff_t;

typedef struct {
    uint8_t  packet[RX_PACKET_SIZE];
    uint16_t panId; //синхрослово
    int8_t   rssi; //уровень сигнала (нужно передать координатору) генерируется драйвером на споте
    uint8_t  lqi;
    freqoff_t freqoff;
}radioReceive_t; //134 байта

#endif //SPOT_CPP_RADIO_RECEIVE_H
