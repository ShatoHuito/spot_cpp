#include "main.h"
#include "defines.h"
#include "device_class/device.h"
#include "packets/packets.h"
#include "spot_class/spot.h"
#include "radio_receive/radio_receive.h"

spot spot_unit;

void pollin_handler(int sig)
{
    spot_unit.isReceivedPacket = true;
    memset(&spot_unit.buf, 0, sizeof (radioReceive_t));
    read(spot_unit.get_radio_device_fd(), &spot_unit.buf, sizeof (radioReceive_t));
    if(spot_unit.buf.packet[0] != 8)
        memset(&spot_unit.buf, 0, sizeof (radioReceive_t));
}

int main() {
    spot_unit.connect_to_radio_device(RADIO_DEVICE_DRIVER_PATH); //соединение с узлом
    spot_unit.connect_to_master(PORT, SERVER_IP); //соединение с мастером

    //обработчик события от узла
    signal(SIGIO, pollin_handler);

    spot_unit.start_message_exchande();

    close(spot_unit.get_radio_device_fd());
    return 0;
}
