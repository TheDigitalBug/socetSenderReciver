
#include "serClient.h"



int main(/*int argc, const char * argv[]*/)
{
	// 1. Create socket for incoming connections
	// AF_INET     -> iPv4
	// SOCK_STREAM -> TCP type of communitation
	// masterSocet -> socket descriptor
	int		masterSocket;
	masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (masterSocket < 0)
		errorMsg("socket() failed");


	// 2. Dlind. Set address and port to server
	// htons() / htonl() ->convert to shor whith networt endians ???
	// INADDR_LOOPBACK   -> 127.0.0.1
	// INADDR_ANY        -> 0.0.0.0
	
	struct	sockaddr_in socketAddr;
	
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12345);
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int		status;
	status = bind(masterSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr));
	
	if (status < 0)
		errorMsg("bind() failed");
	
	
	// 3. Listen. Mark the socket so it will listen for incoming connections
	// SOMAXCONN -> 128 max of incomming conections at queque, rest will be canceled
	status = listen(masterSocket, SOMAXCONN);
	if (status < 0)
		errorMsg("listen() failed");
	
	printf("SERVER is started\n");
	printf("Wait for incoming inforamtion:\n");
	
	// 4. Accept.
	int		slaveSocket;
	while(1)
	{
		//                                    | *clientAddr тут адресс и порт откуда пришел клиент
		//                                       | *sizeof(clientAddr)
		char	buf[255] = {0}; //255 MAX_NAME
		slaveSocket = accept(masterSocket, 0, 0);
		size_t recived;
		
		recived = recv(slaveSocket, buf, 3, 0);
		printf("%s\n", buf);
		
		int fd = open(buf, O_RDWR|O_TRUNC|O_CREAT);
		
		if (fd < 0)
			errorMsg("ERROR:can't create file");
		
		while((recived = recv(slaveSocket, buf, 5, 0)) > 0)
		{
			
			send(slaveSocket, buf, 5, 0);
			
			write(1, &buf, 5);
			write(fd, &buf, 5);
			bzero(buf, 100);
			
			
		}
		close(fd);
		//		shutdown(slaveSocket, SHUT_RDWR);
		//		close(slaveSocket);
printf("\n");
		
	
	}

	
	
	
	return 0;
}
