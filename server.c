
#include "serClient.h"

int		getFileName(int slaveSocket)
{
	int		fd;
	char		buf[NAME_MAX];

	if (recv(slaveSocket, buf, NAME_MAX, 0) < 0)
		errorMsg("Can't recieve file name");
	fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		errorMsg("can't create file");
	return (fd);
}

int		main(/*int argc, const char * argv[]*/)
{
	// 1. Create socket for incoming connections
	int		masterSocket;
	masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (masterSocket < 0)
		errorMsg("socket() failed");


	// 2. Blind. Set address and port to server
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
	printf("Waiting...\n");
	
	// 4. Accept.
	int		slaveSocket;
	char		buf[READ_SIZE];
	size_t	recived;
	int		fd;
	while(1)
	{
		slaveSocket = accept(masterSocket, 0, 0);
		fd = getFileName(slaveSocket);
		
		while((recived = recv(slaveSocket, buf, READ_SIZE - 1, 0)) > 0)
		{
			write(fd, &buf, READ_SIZE - 1); ///UNICODE
			bzero(buf, 100);
		}
		close(fd);
		printf("[File Recieved!]\n");
		printf("Waiting...\n");
		//		shutdown(slaveSocket, SHUT_RDWR);
		//		close(slaveSocket);
	}
	return 0;
}
