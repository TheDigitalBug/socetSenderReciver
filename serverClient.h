
#ifndef serverClient_h
#define serverClient_h

#define READ_SIZE 256
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RESET   "\x1b[0m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/syslimits.h> // for NAME_MAX
#include <fcntl.h>
#include <sys/stat.h>

void		errorMsg(char *str);
void		sendFileName(int clientSocket, char *argv);
int		getFileName(int slaveSocket);

#endif
