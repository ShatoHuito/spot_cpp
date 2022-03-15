//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_CONNECT_TO_MASTER_H
#define SPOT_CPP_CONNECT_TO_MASTER_H

#include "../device_class/device.h"
#include "../network/socket_src.h"
#include "../main.h"

bool connect_to_master(struct pollfd *ufds);

#endif //SPOT_CPP_CONNECT_TO_MASTER_H
