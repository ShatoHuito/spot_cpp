//
// Created by ilnur on 10.03.2022.
//

#include "message_exchange.h"
#include "../test_func/test_func.h"
#include "../crc/crc.h"
#include "../main.h"
#include <sys/poll.h>


extern int	radio_device_fd; //fd для узла
extern uint16_t master_fd;
extern bool isReceivedPacket;
extern radioReceive_t buf;

//bool start_message_exchande(spot spot, struct pollfd *ufds){
//    packet_device_spot_t packet;
//    uint8_t byte_arr[sizeof(packet_device_spot_t)];
//    uint8_t byte_arr_master_slave[sizeof(packet_spot_master_t)];
//    packet_device_spot_t *tmp_packet;
//    int i = 0;
//
//    while (radio_device_fd > 0) {
//        if (isReceivedPacket) {
//            printf("\n  ->  count = %d\n", i++);
//            isReceivedPacket = false;
//            spot.read_packet(&packet, buf.packet);
//            print_packet(packet);
//            if (packet.length != 8 && !check_CRC(&packet)) {
//                memset(&packet, 0, sizeof(packet_device_spot_t));
//                continue;
//            }
//            if (check_CRC(&packet)) {
//                spot.send_packet_to_master(packet, buf.rssi, master_fd);
//                tmp_packet = spot.get_answer_from_queue(&packet);
//                if (tmp_packet) {
//                    memcpy(byte_arr, tmp_packet, sizeof(packet_device_spot_t));
//                    write(radio_device_fd, byte_arr, sizeof(packet_device_spot_t));
//                    printf("---------SEND TO NODE----");
//                    print_small_paket(byte_arr);
//                    spot.send_packet_to_master(packet, buf.rssi, master_fd);
//                }
//            }
//        }
//        poll(reinterpret_cast<pollfd *>(&ufds), 1, 300);
//        if (ufds->revents && POLLIN) {
//            recv(master_fd, byte_arr_master_slave, BUFF_SIZE, 0);
//            spot.prepare_answer_spot_node(byte_arr_master_slave, &packet);
//        }
//        usleep(1);
//    }
//    return false;
//}