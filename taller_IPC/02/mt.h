#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define PORT 5555
#define MAX_MSG_LENGTH 1024
#define END_STRING "chau\n"
#define COMPLETE_STRING "fin-respuesta"
#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2


