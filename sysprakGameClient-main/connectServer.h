#ifndef serverConnect
#define serverConnect

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h> //file control options ... socket non-blocking etc

#define TIMEOUT 3
int connectServer(int port, char *host);
#endif
