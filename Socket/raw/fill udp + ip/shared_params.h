#ifndef SARED_PARAMS_H
#define SARED_PARAMS_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

#define MAX_CLIENTS 5
#define BUFFER_SIZE 512
#define SERVER_IP INADDR_LOOPBACK
#define CLIENT_IP INADDR_LOOPBACK
#define SERVER_PORT 55555
#define CLIENT_PORT 44444

#define SOCK_NAME_SER "/tmp/sock_name_serv"
#define SOCK_NAME_CLI "/tmp/sock_name_clie"

//  Выбор между работой в пределах одной машины или через сеть
//  0 - в пределах одной машины
//  1 - через IPv4
#define VIA_IPV_4 1

//  Выбор типа сокета
//  0 - SOCK_STREAM
//  1 - SOCK_DEGRAM
#define SOCK_TYPE_OPT 1

#if VIA_IPV_4 == 0

#define DOMAIN_TYPE AF_LOCAL

#else

#define DOMAIN_TYPE AF_INET

#endif

#if SOCK_TYPE_OPT == 0

#define SOCK_TYPE SOCK_STREAM

#else

#define SOCK_TYPE SOCK_DGRAM

#endif

#endif
