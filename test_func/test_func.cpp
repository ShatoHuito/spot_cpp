//
// Created by ilnur on 10.03.2022.
//
#include "test_func.h"


///тестовая функция
bool print_small_paket(uint8_t *byte_arr)
{
    uint8_t i = 0;

    printf("\n---------PACKET------------\n");
    printf ("length = %d", byte_arr[i]);
    printf ("   Counter = %d", (byte_arr[++i] & 0b11));
    printf ("   Addres = %d", ((byte_arr[i] >> 2) | (byte_arr[++i] << 6)));
    printf ("   Info = %d", byte_arr[++i] | byte_arr[++i] << 8 | byte_arr[++i] << 16 | byte_arr[++i] << 24);
    printf("    Crc = %d", (byte_arr[++i] | byte_arr[++i] << 8 ));
    printf("\n--------   END   -------------\n");

    return false;
}

bool print_packet(packet_device_spot_t pack){

    printf("\n---------PACKET------------\n");
    printf("length = %d", pack.length);
    printf("   counter = %d", pack.counter);
    printf("   address = %d", pack.address);
    printf("    info %d", pack.info);
    printf("    crc %d", pack.crc);
    printf("\n--------   END   -------------\n");

    return false;
}