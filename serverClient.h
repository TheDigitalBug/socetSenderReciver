
#ifndef serverClient_h
#define serverClient_h

#define READ_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <sys/syslimits.h> // for NAME_MAX


#include <fcntl.h>
#include <sys/stat.h>



void		errorMsg(char *str);
void		sendFileName(int clientSocket, char *argv);
int		getFileName(int slaveSocket);

#endif
