
#include "serClient.h"

int		getFileName(int slaveSocket)
{
	int		fd;
	char		buf[NAME_MAX] = {0};

	if (recv(slaveSocket, buf, NAME_MAX, 0) < 0)
		errorMsg("Can't recieve file name");
	fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		errorMsg("can't create file");
	printf("[File '%s' ", buf);
	return (fd);
}

int		main(/*int argc, const char * argv[]*/)
{

	int		masterSocket;
	struct	sockaddr_in socketAddr;
	int		status;
	int		slaveSocket;
	char		buf[READ_SIZE];
	size_t	recived;
	int		fd;

	// 1. Create socket for incoming connections
	masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (masterSocket < 0)
		errorMsg("socket() failed");

	// 2. Blind. Set address and port to server
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12345);
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	status = bind(masterSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr));
	if (status < 0)
		errorMsg("bind() failed");
	
	// 3. Listen. Mark the socket so it will listen for incoming connections
	status = listen(masterSocket, SOMAXCONN);
	if (status < 0)
		errorMsg("listen() failed");
	
	printf("SERVER is started\n");
	printf("Waiting...\n");
	
	// 4. Accept.
	while(1)
	{
		slaveSocket = accept(masterSocket, 0, 0);
		fd = getFileName(slaveSocket);
		while((recived = recv(slaveSocket, buf, READ_SIZE - 1, 0)) > 0)
		{
			write(fd, &buf, recived); ///UNICODE
			bzero(buf, READ_SIZE - 1);
		}
		close(fd);
		shutdown(slaveSocket, SHUT_RDWR);
		printf("Recieved!]\n");
		printf("Waiting...\n");
	}
	return 0;
}
