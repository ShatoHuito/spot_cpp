//
// Created by ilnur on 10.03.2022.
//

#include <netinet/in.h>
#include "connect_to_master.h"

extern uint16_t master_fd;

//bool connect_to_master(struct pollfd *ufds){
//    struct sockaddr_in adr = {0};
//    master_fd = Socket(AF_INET, SOCK_STREAM, 0);
//    adr.sin_family = AF_INET;
//    adr.sin_port = htons(PORT);
//    Inet_pton(AF_INET, SERVER_IP, &adr.sin_addr); //conver from text to binary
//    Connect(master_fd, (struct sockaddr *) &adr, sizeof adr);
//    ufds->fd = master_fd;
//    ufds->events = POLLIN;
//    return false;
//};
