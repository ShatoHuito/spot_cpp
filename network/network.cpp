//
// Created by ilnur on 10.03.2022.
//

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "network.h"

int Network::Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if (res == -1) {
        perror("socket fail");
        exit(EXIT_FAILURE);
    }
    return res;
}

//привязывает адресс и порт к сокету
void Network::Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = bind(sockfd, addr, addrlen);
    if (res == -1){
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
}

//слушает по сокету
void Network::Listen(int sockfd, int backlog){
    int res = listen(sockfd, backlog);
    if (res == -1){
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
}

//возвращает дескриптор только что подключенного сокета
int Network::Accept(int sockfd, struct  sockaddr *addr, socklen_t *addrlen){
    int res = accept(sockfd, addr, addrlen);
    if (res == -1){
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    return res;
}

size_t Network::Read(int fd, void *buf, size_t count){
    ssize_t res = read(fd, buf, count);
    if (res == -1){
        perror("read fail");
        exit(EXIT_FAILURE);
    }
    if (res == 0)
        printf("end of read file\n");
    return 0;
}

//подключение к серверу
void Network::Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){
    int res = connect(sockfd, addr, addrlen);
    if (res == -1){
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
}

void Network::Inet_pton(int af, const char *src, void *dst){
    int res = inet_pton(af, src, dst);
    if (res == 0){
        printf("src does not contain a character string representing a valid network address in the specified address family\n");
        exit(EXIT_FAILURE);
    }
    if (res == -1){
        perror("inet_pron fail");
        exit(EXIT_FAILURE);
    }
}

