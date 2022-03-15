//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_SPOT_H
#define SPOT_CPP_SPOT_H

#include <cstdint>
#include <sys/poll.h>
#include <fcntl.h>
#include "../defines.h"
#include "../packets/packets.h"
#include "../device_class/device.h"
#include "../radio_receive/radio_receive.h"

class Spot{
public:
    bool            isReceivedPacket;
    radioReceive_t  buf;
    struct pollfd ufds{};
    device_info slaves[MAX_SLAVES_COUNT];

    Spot();

    uint16_t get_radio_device_fd();

    void connect_to_radio_device(char *path_to_driver);
    void connect_to_master(uint16_t port, char *server_ip);

    void start_message_exchande();

    bool read_packet(packet_device_spot_t *packet, const uint8_t *buff);
    bool read_packet_master_node(packet_device_spot_t *packet, const uint8_t *buff);
    bool send_packet_to_master(packet_device_spot_t sub_packet, uint8_t device_rssi, uint16_t master_fd);
    packet_device_spot_t *get_answer_from_queue(packet_device_spot_t *rcv_packet);
    bool prepare_answer_spot_node(uint8_t *byte_arr_master_node, packet_device_spot_t *packet_tx);

    bool handle_rx_packet(packet_device_spot_t *rcv_packet);
    bool handle_init_packet(packet_device_spot_t *rcv_packet);



private:
    uint16_t    address;
    uint16_t 	radio_device_fd; //fd для узла
    uint16_t    master_fd;
};

#endif //SPOT_CPP_SPOT_H
