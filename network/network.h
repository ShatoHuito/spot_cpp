//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_NETWORK_H
#define SPOT_CPP_NETWORK_H


#include <sys/socket.h>

class Network{
public:
    static int Socket(int domain, int type, int protocol);

    void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

    void Listen(int sockfd, int backlog);

    int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

    size_t Read(int fd, void *buf, size_t count);

    static void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

    static void Inet_pton(int af, const char *src, void *dst);

};

#endif //SPOT_CPP_NETWORK_H
