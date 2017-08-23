
#include "serClient.h"

void		sendFileName(int clientSocket, char *argv)
{
	// remove path to file from file name
	char		*name;

	name = strrchr(argv, '/') + 1;
	printf("File '%s' ", name);
	if (send(clientSocket, name, strlen(name), 0) <= 0)
		errorMsg("can't send file");
}

int main(int argc, char *argv[])
{
	char		buf[READ_SIZE] = {0};
	int		fd;
	int		clientSocket;
	struct	sockaddr_in socketAddr;
	int		status;
	size_t	readed;
	size_t	sended;

	if (argc != 2)
		errorMsg("Wrong number of parameters!");
	
	fd = open(argv[1], O_RDONLY);
	if ((read(fd, buf, 0) == -1) || (fd < 0))
		errorMsg("Can't open file!");

	// 1. Create socket for incoming connections
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket < 0)
		errorMsg("socket() failed");
	
	// 2. set IP/PORT of Server
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(12345);
	socketAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	status = connect(clientSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr));
	if (status < 0)
		errorMsg("connect() failed");

	sendFileName(clientSocket, argv[1]);

	while ((readed = read(fd, buf, READ_SIZE - 1)) > 0)
	{
		sended = send(clientSocket, buf, readed, 0);
		if (sended <= 0)
			errorMsg("can't send file");
	}
	close(fd);
	printf("successfully send!\n");
	shutdown(clientSocket, SHUT_RDWR);
	return 0;
}
