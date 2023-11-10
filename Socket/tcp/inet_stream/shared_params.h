#ifndef SARED_PARAMS_H
#define SARED_PARAMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCK_NAME "/tmp/sock_name"
#define PORT_NUM 55555

//  Выбор между работой в пределах одной машины или через сеть
//  0 - в пределах одной машины
//  1 - через IPv4
#define VIA_IPV_4 1

//  Выбор типа сокета
//  0 - SOCK_STREAM
//  1 - SOCK_DEGRAM
#define SOCK_TYPE_OPT 0

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