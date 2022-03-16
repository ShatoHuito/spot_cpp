//
// Created by ilnur on 15.03.2022.
//

#include <cstdint>
#include <cstring>
#include <sys/socket.h>
#include "packets.h"
#include "../crc/crc.h"
#include "../device_class/device.h"

bool Packet::read_packet(packet_device_spot_t *packet, const uint8_t *buff) {
    packet->length = buff[0];
    packet->counter = buff[1] & 0b11;
    packet->address = (buff[2] << 6) | (buff[1] >> 2);
    packet->info = buff[3] | buff[4] << 8 | buff[5] << 16 | buff[6] << 24;
    packet->crc = buff[7] | buff[8] << 8;
    return false;
}

bool Packet::read_packet_master_node(packet_device_spot_t *packet, const uint8_t *buff) {
    uint8_t i = 4;
    uint8_t len_sub_packet = buff[i++];

    ///достаем из пакета МАСТЕР-СПОТ и упаковываем пакет СПОТ-УЗЕЛ
    packet->length = len_sub_packet;
    packet->counter = buff[i] & 0b11;
    packet->address = (buff[i] >> 2) | (buff[++i] << 6);
    packet->info = buff[++i] | buff[++i] << 8 | buff[++i] << 16 | buff[++i] << 24;
    packet->crc = buff[++i] | buff[++i] << 8;
    return false;
}

/**
 * Функция отправляет пакет мастеру, который содержит пакет от узла
 *
 * @param sub_packet структура с пакетом от узла для спота
 * @param device_rssi rssi узла от которого получено сообщение
 * @param master_fd fd мастер устройства
 * @return всегда возвращает false
 */

bool Packet::send_packet_to_master(packet_device_spot_t sub_packet, uint8_t device_rssi, uint16_t address, uint16_t master_fd) {
    packet_spot_master_t packet;
    uint8_t byte_arr[sizeof(packet_spot_master_t)];

    ///создаем пакет МАСТЕР-СПОТ
    packet.address = address;
    packet.length = sizeof(packet_spot_master_t);
    packet.rssi = device_rssi;
    packet.sub_packet = sub_packet;
    packet.crc = 0;
    packet.crc = dallasCrc16((uint8_t *)&packet, (packet.length - 1));

    ///отправляем мастеру
    memcpy(byte_arr, &packet, sizeof(packet_spot_master_t));
    //  print_paket(byte_arr, "SPOT SEND TO MASTER");
    send(master_fd, byte_arr, sizeof(packet_spot_master_t), 0);
    return false;
}

/**
 * Функция обрабатывает запрос в рамках открытого сеанса, кладет в очередь пакет-ответ для узла
 * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
 * @param rcv_packet адрес полученного пакета запроса
 * @return всегда возвращает 0, проверок никаких здесь не проводится
 */

bool Packet::handle_rx_packet(packet_device_spot_t *rcv_packet, Spot *spot)
{
    packet_device_spot_t	*packet_to_update;
    device_info	*device = nullptr;
    uint8_t		i = 0;

    while (i < MAX_SLAVES_COUNT && spot->slaves[i].address)
    {
        if (spot->slaves[i].address == rcv_packet->address)
        {
            device = &spot->slaves[i];
            break;
        }
        i++;
    }
    if (device)
    {
        packet_to_update = &(device->queue[rcv_packet->counter].packet);
        packet_to_update->counter = rcv_packet->counter;
        packet_to_update->address = rcv_packet->address;
        packet_to_update->length = sizeof(packet_device_spot_t) - 1;
        packet_to_update->info = rcv_packet->info;
        // для исходящего пакета контрольная сумма инвертирована
        packet_to_update->crc = 0;
        packet_to_update->crc = ~(dallasCrc16((uint8_t *) packet_to_update, (packet_to_update->length - 1)));
    }
    return (false);
}

/**
 * Функция обрабатывает инициализационный запрос (начало сеанса), кладет в очередь пакет-ответ для узла
 * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
 * @param rcv_packet адрес полученного пакета запроса
 * @return всегда возвращает 0, проверок никаких здесь не проводится
 */

bool Packet::handle_init_packet(packet_device_spot_t *rcv_packet, Spot *spot)
{
    packet_device_spot_t	*packet_to_update;
    device_info	*device = nullptr;
    uint8_t		i = 0;
    uint32_t	counter;

    while (i < MAX_SLAVES_COUNT && \
        spot->slaves[i].address && \
		spot->slaves[i].address != rcv_packet->address)
        i++;
    if (i < MAX_SLAVES_COUNT)
    {
        device = &spot->slaves[i];
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