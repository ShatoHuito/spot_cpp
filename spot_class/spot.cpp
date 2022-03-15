//
// Created by ilnur on 10.03.2022.
//

#include <cstring>
#include <fcntl.h>
#include "spot.h"
#include "../crc/crc.h"
#include "../network/network.h"
#include "../test_func/test_func.h"

Spot::Spot() {
    this->address = SPOT_ADDRESS;
    this->isReceivedPacket = false;
}

/**
 * Функция находит в очереди пакет-ответ для узла
 * @param rcv_packet адрес полученного пакета запроса
 * @return возвращает адрес пакета, NULL если валидного ответа нет
 */

packet_device_spot_t *Spot::get_answer_from_queue(packet_device_spot_t *rcv_packet) {
    uint8_t i = 0;
    uint32_t counter;

    while (i < MAX_SLAVES_COUNT && this->slaves[i].address)
    {
        if (this->slaves[i].address == rcv_packet->address)
        {
            counter = this->slaves[i].counter;
            if (rcv_packet->counter == DEFAULT_COUNTER)
                return &(this->slaves[i].queue[counter % 3].packet);
            else if ((counter + 1) % 3 == rcv_packet->counter)
            {
                this->slaves[i].counter++;
                this->slaves[i].queue[(counter) % 3].packet.counter = counter % 3;
                this->slaves[i].queue[(counter) % 3].packet.crc = 0;
                this->slaves[i].queue[(counter) % 3].packet.crc = \
				~(dallasCrc16((uint8_t *) &this->slaves[i].queue[(counter) % 3].packet,
                              this->slaves[i].queue[(counter) % 3].packet.length - 1));
                //обновить CRC???
                return &(this->slaves[i].queue[(counter) % 3].packet);
            } else
                return &(this->slaves[i].queue[(counter - 1) % 3].packet);
        }
        i++;
    }
    return nullptr;
}

///**
// * Функция обрабатывает запрос в рамках открытого сеанса, кладет в очередь пакет-ответ для узла
// * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
// * @param rcv_packet адрес полученного пакета запроса
// * @return всегда возвращает 0, проверок никаких здесь не проводится
// */
//
//bool Spot::handle_rx_packet(packet_device_spot_t *rcv_packet)
//{
//    packet_device_spot_t	*packet_to_update;
//    device_info	*device = nullptr;
//    uint8_t		i = 0;
//
//    while (i < MAX_SLAVES_COUNT && this->slaves[i].address)
//    {
//        if (this->slaves[i].address == rcv_packet->address)
//        {
//            device = &this->slaves[i];
//            break;
//        }
//        i++;
//    }
//    if (device)
//    {
//        packet_to_update = &(device->queue[rcv_packet->counter].packet);
//        packet_to_update->counter = rcv_packet->counter;
//        packet_to_update->address = rcv_packet->address;
//        packet_to_update->length = sizeof(packet_device_spot_t) - 1;
//        packet_to_update->info = rcv_packet->info;
//        // для исходящего пакета контрольная сумма инвертирована
//        packet_to_update->crc = 0;
//        packet_to_update->crc = ~(dallasCrc16((uint8_t *) packet_to_update, (packet_to_update->length - 1)));
//    }
//    return (false);
//}

/**
 * Функция обрабатывает инициализационный запрос (начало сеанса), кладет в очередь пакет-ответ для узла
 * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
 * @param rcv_packet адрес полученного пакета запроса
 * @return всегда возвращает 0, проверок никаких здесь не проводится
 */

bool Spot::handle_init_packet(packet_device_spot_t *rcv_packet)
{
    packet_device_spot_t	*packet_to_update;
    device_info	*device = nullptr;
    uint8_t		i = 0;
    uint32_t	counter;

    while (i < MAX_SLAVES_COUNT && \
        this->slaves[i].address && \
		this->slaves[i].address != rcv_packet->address)
        i++;
    if (i < MAX_SLAVES_COUNT)
    {
        device = &this->slaves[i];
        device->address = rcv_packet->address;
        counter = RANDOM_COUNTER;
        device->counter = counter;
        packet_to_update = &(device->queue[counter % 3].packet);
        packet_to_update->counter = rcv_packet->counter;
        packet_to_update->address = rcv_packet->address;
        packet_to_update->length = sizeof(packet_device_spot_t) - 1;
        packet_to_update->info = counter;
        packet_to_update->crc = 0;
        // для исходящего пакета контрольная сумма инвертирована
        packet_to_update->crc = ~(dallasCrc16((uint8_t *) packet_to_update, (packet_to_update->length - 1)));
    }
    return (false);
}


/**
 * Функция обрабатывает полученный запрос и подготавливает на него ответ, если запрос валидный
 *
 * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
 * @param packet_tx адрес пакета, в который записывается необходимая для передачи информация
 * @return возвращает true, если полученный запрос валидный
 */

bool Spot::prepare_answer_spot_node(uint8_t *byte_arr_master_node, packet_device_spot_t *packet_tx)
{
    packet_device_spot_t rcv_packet;

   Packet::read_packet_master_node(&rcv_packet, byte_arr_master_node);
    if (rcv_packet.counter == DEFAULT_COUNTER)
        handle_init_packet(&rcv_packet);
    else
        Packet::handle_rx_packet(&rcv_packet, this);
    return false;
}

void Spot::connect_to_radio_device(char *path_to_driver) {
    this->radio_device_fd = open(RADIO_DEVICE_DRIVER_PATH, O_RDWR);
}

void Spot::connect_to_master(uint16_t port, char *server_ip) {
    struct sockaddr_in adr = {0};
    this->master_fd = Network::Socket(AF_INET, SOCK_STREAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    Network::Inet_pton(AF_INET, SERVER_IP, &adr.sin_addr); //conver from text to binary
    Network::Connect(master_fd, (struct sockaddr *) &adr, sizeof adr);
    this->ufds.fd = master_fd;
    this->ufds.events = POLLIN;
}

uint16_t Spot::get_radio_device_fd() {
    return this->radio_device_fd;
}

void Spot::start_message_exchande() {
    packet_device_spot_t packet;
    uint8_t byte_arr[sizeof(packet_device_spot_t)];
    uint8_t byte_arr_master_slave[sizeof(packet_spot_master_t)];
    packet_device_spot_t *tmp_packet;
    int i = 0;

    while (radio_device_fd > 0) {
        if (isReceivedPacket) {
            printf("\n  ->  count = %d\n", i++);
            this->isReceivedPacket = false;
            Packet::read_packet(&packet, buf.packet);
            print_packet(packet);
            if (packet.length != 8 && !check_CRC(&packet)) {
                memset(&packet, 0, sizeof(packet_device_spot_t));
                continue;
            }
            if (check_CRC(&packet)) {
                Packet::send_packet_to_master(packet, buf.rssi, address, master_fd);
                tmp_packet = this->get_answer_from_queue(&packet);
                if (tmp_packet) {
                    memcpy(byte_arr, tmp_packet, sizeof(packet_device_spot_t));
                    write(radio_device_fd, byte_arr, sizeof(packet_device_spot_t));
                    printf("---------SEND TO NODE----");
                    print_small_paket(byte_arr);
                    Packet::send_packet_to_master(packet, buf.rssi, address, master_fd);
                }
            }
        }
        poll(&ufds, 1, 300);
        if (this->ufds.revents && POLLIN) {
            recv(master_fd, byte_arr_master_slave, BUFF_SIZE, 0);
            this->prepare_answer_spot_node(byte_arr_master_slave, &packet);
        }
        usleep(1);
    }
}



