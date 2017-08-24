
#include "serverClient.h"

int				getFileName(int slaveSocket)
{	
	int		fd;
	char		buf[NAME_MAX + 9] = {0};

	strcat(buf, "received/");
	if (recv(slaveSocket, buf + 9, NAME_MAX, 0) < 0)
		errorMsg("Can't recieve file name");
	system("mkdir -p received");	
	remove(buf);
	fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		errorMsg("can't create file");
	printf("[File '%s' ", buf);
	return (fd);
}

static void		setServerInfo(struct sockaddr_in *socketAddr)
{
	socketAddr->sin_family = AF_INET;
	socketAddr->sin_port = htons(SERVER_PORT);
	//	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, SERVER_IP, &(socketAddr->sin_addr));
}

int				main(void)
{

	int					masterSocket;
	struct sockaddr_in	socketAddr;
	int					slaveSocket;
	char					buf[READ_SIZE];
	size_t				recived;
	int					fd;
	struct sockaddr_in	connectedClientAddr; // to get ip of client
	unsigned int			sizeClientAddr;
	char					strClientAddr[INET_ADDRSTRLEN];


	// 1. Create socket for incoming connections
	if ((masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		errorMsg("socket() failed");

	// 2. Set address and port to server. Bind.
	setServerInfo(&socketAddr);
	if ((bind(masterSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr))) < 0)
		errorMsg("bind() failed");
	
	// 3. Listen. Mark the socket so it will listen for incoming connections
	if (listen(masterSocket, SOMAXCONN) < 0)
		errorMsg("listen() failed");
	
	printf("SERVER [%s : %d] started\n", SERVER_IP, SERVER_PORT);
	printf("Waiting...\n");
	
	// 4. Accept.
	while(1)
	{
		// 5. get client IP
		sizeClientAddr = sizeof(connectedClientAddr);
		slaveSocket = accept(masterSocket, (struct sockaddr *) &(connectedClientAddr), &sizeClientAddr);
		inet_ntop(AF_INET, &(connectedClientAddr.sin_addr.s_addr), strClientAddr, INET_ADDRSTRLEN);
		printf(COLOR_GREEN"Client [%s] connected\n"COLOR_RESET, strClientAddr);
		
		// 6. get file name
		fd = getFileName(slaveSocket);
		
		while((recived = recv(slaveSocket, buf, READ_SIZE - 1, 0)) > 0)
		{
			write(fd, &buf, recived); ///UNICODE
			bzero(buf, READ_SIZE - 1);
		}
		close(fd);
		shutdown(slaveSocket, SHUT_RDWR);
		printf("Recieved!]\n");
		printf(COLOR_RED"Client [%s] disconnected\n\n"COLOR_RESET, strClientAddr);
		printf("Waiting for new connection...\n");
	}
	return 0;
}
