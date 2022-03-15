//
// Created by ilnur on 10.03.2022.
//

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "spot.h"
#include "../crc/crc.h"
#include "../network/socket_src.h"
#include "../test_func/test_func.h"

spot::spot() {
    this->address = SPOT_ADDRESS;
    this->isReceivedPacket = false;
}

bool spot::read_packet(packet_device_spot_t *packet, const uint8_t *buff) {
    packet->length = buff[0];
    packet->counter = buff[1] & 0b11;
    packet->address = (buff[2] << 6) | (buff[1] >> 2);
    packet->info = buff[3] | buff[4] << 8 | buff[5] << 16 | buff[6] << 24;
    packet->crc = buff[7] | buff[8] << 8;
    return false;
}

bool spot::read_packet_master_node(packet_device_spot_t *packet, const uint8_t *buff) {
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

bool spot::send_packet_to_master(packet_device_spot_t sub_packet, uint8_t device_rssi, uint16_t master_fd) {
    packet_spot_master_t packet;
    uint8_t byte_arr[sizeof(packet_spot_master_t)];

    ///создаем пакет МАСТЕР-СПОТ
    packet.address = this->address;
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
 * Функция находит в очереди пакет-ответ для узла
 * @param rcv_packet адрес полученного пакета запроса
 * @return возвращает адрес пакета, NULL если валидного ответа нет
 */

packet_device_spot_t *spot::get_answer_from_queue(packet_device_spot_t *rcv_packet) {
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

/**
 * Функция обрабатывает запрос в рамках открытого сеанса, кладет в очередь пакет-ответ для узла
 * @param spotDeviceInfo структура с данными спота и очередями готовых пакетов для узлов
 * @param rcv_packet адрес полученного пакета запроса
 * @return всегда возвращает 0, проверок никаких здесь не проводится
 */

bool spot::handle_rx_packet(packet_device_spot_t *rcv_packet)
{
    packet_device_spot_t	*packet_to_update;
    device_info	*device = nullptr;
    uint8_t		i = 0;

    while (i < MAX_SLAVES_COUNT && this->slaves[i].address)
    {
        if (this->slaves[i].address == rcv_packet->address)
        {
            device = &this->slaves[i];
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

bool spot::handle_init_packet(packet_device_spot_t *rcv_packet)
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

bool spot::prepare_answer_spot_node(uint8_t *byte_arr_master_node, packet_device_spot_t *packet_tx)
{
    packet_device_spot_t rcv_packet;

    this->read_packet_master_node(&rcv_packet, byte_arr_master_node);
    if (rcv_packet.counter == DEFAULT_COUNTER)
        handle_init_packet(&rcv_packet);
    else
        handle_rx_packet(&rcv_packet);
    return false;
}

void spot::connect_to_radio_device(char *path_to_driver) {
    this->radio_device_fd = open(RADIO_DEVICE_DRIVER_PATH, O_RDWR);
}

void spot::connect_to_master(uint16_t port, char *server_ip) {
    struct sockaddr_in adr = {0};
    this->master_fd = Socket(AF_INET, SOCK_STREAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    Inet_pton(AF_INET, SERVER_IP, &adr.sin_addr); //conver from text to binary
    Connect(master_fd, (struct sockaddr *) &adr, sizeof adr);
    this->ufds.fd = master_fd;
    this->ufds.events = POLLIN;
}

uint16_t spot::get_radio_device_fd() {
    return this->radio_device_fd;
}

void spot::start_message_exchande() {
    packet_device_spot_t packet;
    uint8_t byte_arr[sizeof(packet_device_spot_t)];
    uint8_t byte_arr_master_slave[sizeof(packet_spot_master_t)];
    packet_device_spot_t *tmp_packet;
    int i = 0;

    while (radio_device_fd > 0) {
        if (isReceivedPacket) {
            printf("\n  ->  count = %d\n", i++);
            this->isReceivedPacket = false;
            this->read_packet(&packet, buf.packet);
            print_packet(packet);
            if (packet.length != 8 && !check_CRC(&packet)) {
                memset(&packet, 0, sizeof(packet_device_spot_t));
                continue;
            }
            if (check_CRC(&packet)) {
                send_packet_to_master(packet, buf.rssi, master_fd);
                tmp_packet = this->get_answer_from_queue(&packet);
                if (tmp_packet) {
                    memcpy(byte_arr, tmp_packet, sizeof(packet_device_spot_t));
                    write(radio_device_fd, byte_arr, sizeof(packet_device_spot_t));
                    printf("---------SEND TO NODE----");
                    print_small_paket(byte_arr);
                    this->send_packet_to_master(packet, buf.rssi, master_fd);
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



