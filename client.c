
#include "serClient.h"

int main(int argc, char *argv[])
{
	char		buf[10];
	int		fd;
	if (argc != 2)
		errorMsg("ERROR: Wrong number of parameters!");
	
	
	fd = open(argv[1], O_RDONLY);
	if ((read(fd, buf, 0) == -1) || (fd < 0))
		errorMsg("ERROR: Can't open file!");

	// 1. Create socket for incoming connections
	// AF_INET     -> iPv4
	// SOCK_STREAM -> TCP type of communitation
	// masterSocet -> socket descriptor
	int		clientSocket;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (clientSocket < 0)
		errorMsg("socket() failed");
	
	// 2.
	// htons() / htonl() ->convert to shor whith networt endians ???
	// INADDR_LOOPBACK   -> 127.0.0.1
	
	struct	sockaddr_in socketAddr;
	
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12345);
	socketAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	int		status;
	status = connect(clientSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr));
	
	if (status < 0)
		errorMsg("connect() failed");
	
	size_t	readed;
	size_t	sended;

	
	//---send file name
	sended = send(clientSocket, argv[1], strlen(argv[1]), 0);
	if (sended <= 0)
		errorMsg("ERROR: can't send file");
	
	//---send file content
	while ((readed = read(fd, buf, 10)) > 0)
	{
		sended = send(clientSocket, buf, readed, 0);
		printf(">[Client send %zu symbols]\n", sended);
		recv(clientSocket, buf, readed, 0);
	}
	
	close(fd);
	shutdown(clientSocket, SHUT_RDWR);
	//		close(slaveSocket);

	
	
	return 0;
}
