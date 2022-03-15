//
// Created by ilnur on 10.03.2022.
//

#ifndef SPOT_CPP_SOCKET_SRC_H
#define SPOT_CPP_SOCKET_SRC_H


#include <sys/socket.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

size_t Read(int fd, void *buf, size_t count);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src, void *dst);

int is_connection_close(const char *msg);

#endif //SPOT_CPP_SOCKET_SRC_H
