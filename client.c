
#include "serverClient.h"

void		sendFileName(int clientSocket, char *argv)
{
	char		*name;

	// remove path to file from file name
	name = strrchr(argv, '/') + 1;
	printf("File '%s' ", name);
	if (send(clientSocket, name, strlen(name), 0) <= 0)
		errorMsg("can't send file");
}

static void		getServerInfo(struct sockaddr_in *socketAddr, char *ip, char *port)
{
	// get and check server IP/PORT
	unsigned int		serverIp;
	unsigned short	serverPort;
	int				i;

	serverPort = 0;
	if (inet_pton(AF_INET, ip, &serverIp) == 0)
		errorMsg("Wrong IP format!");
	i = 0;
	while(port[i])
	{
		if (port[i] >= '0' && port[i] <= '9')
			serverPort = serverPort * 10 + port[i] - '0';
		else
			errorMsg("Wrong port!");
		i++;
	}
	if (serverPort > SHRT_MAX)
		errorMsg("Wrong port!");
	socketAddr->sin_family = AF_INET;
	socketAddr->sin_port = htons(serverPort);
	inet_pton(AF_INET, ip, &(socketAddr->sin_addr));
}

int main(int argc, char *argv[])
{
	char		buf[READ_SIZE] = {0};
	int		fd;
	int		clientSocket;
	struct	sockaddr_in socketAddr;
	size_t	readed;
	size_t	sended;

	if (argc != 4)
		errorMsg("USAGE: ./client IPv4 port fileToSend");
	
	getServerInfo(&socketAddr, argv[1], argv[2]);
	
	if (((fd = open(argv[3], O_RDONLY)) < 0) || (read(fd, buf, 0) == -1))
		errorMsg("Can't open file!");

	// 1. Create socket for incoming connections
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		errorMsg("socket() failed");
	
	// 2. set IP/PORT of Server. CONNECT
//	setServerInfo(&socketAddr);
	
	if (connect(clientSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr)) < 0)
		errorMsg("connect() failed");
	printf(COLOR_GREEN"Connected to server [%s : %s]\n" COLOR_RESET, argv[1], argv[2]);
	
	sendFileName(clientSocket, argv[3]);

	while ((readed = read(fd, buf, READ_SIZE - 1)) > 0)
	{
		sended = send(clientSocket, buf, readed, 0);
		if (sended <= 0)
			errorMsg("can't send");
	}
	close(fd);
	printf("successfully send!\n");
	printf(COLOR_RED"Disconnected from server [%s : %d] \n"COLOR_RESET, SERVER_IP, SERVER_PORT);
	shutdown(clientSocket, SHUT_RDWR);
	return 0;
}
