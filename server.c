
#include "serverClient.h"

int				getFileName(int slaveSocket)
{	
	int			fd;
	char		buf[NAME_MAX + 9] = {0};

	strcat(buf, "received/");
	if (recv(slaveSocket, buf + 9, NAME_MAX, 0) < 0)
		return (-1);
	system("mkdir -p received");	
	remove(buf);
	fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		return (-1);
	printf("[Receiving file '%s']\n", buf);
	return (fd);
}

static void		setServerInfo(struct sockaddr_in *socketAddr)
{
	socketAddr->sin_family = AF_INET;
	socketAddr->sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &(socketAddr->sin_addr));
}

void			*senderToClient(void *slaveSocketVoid)
{
	int			slaveSocket;
	size_t		recived;
	int			fd;
	char		buf[READ_SIZE];
	struct		sockaddr_in addr;
	
	slaveSocket = *(int *)slaveSocketVoid;
	fd = getFileName(slaveSocket);
	if (fd < 0)
	{
		close(slaveSocket);
		return (0);
	}

	while((recived = recv(slaveSocket, buf, READ_SIZE - 1, 0)) > 0)
	{
		send(slaveSocket, buf, recived, 0);
		write(fd, &buf, recived);
		bzero(buf, READ_SIZE - 1);
	}
	close(fd);

	socklen_t addr_size = sizeof(struct sockaddr_in);
	getpeername(slaveSocket, (struct sockaddr *)&addr, &addr_size);
	
	close(slaveSocket);
	printf(COLOR_RED"Client [%s] disconnected\n"COLOR_RESET, inet_ntoa(addr.sin_addr));
	return 0;
}

int				main(void)
{
	int						masterSocket;
	struct sockaddr_in		socketAddr;
	int						slaveSocket;
	struct sockaddr_in		connectedClientAddr; // to get ip of client
	unsigned int			sizeClientAddr;
	char					strClientAddr[INET_ADDRSTRLEN];
	pthread_t				mainThread;
	int 					*childSlaveSocket;

	if ((masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		errorMsg("socket() failed");

	setServerInfo(&socketAddr);
	if ((bind(masterSocket, (struct sockaddr *) &socketAddr, sizeof(socketAddr))) < 0)
		errorMsg("bind() failed");
	
	if (listen(masterSocket, SOMAXCONN) < 0)
		errorMsg("listen() failed");
	
	printf(COLOR_GREEN"SERVER [%s : %d] started\n"COLOR_RESET, SERVER_IP, SERVER_PORT);
	printf("Waiting...\n");
	
	sizeClientAddr = sizeof(connectedClientAddr);

	while((slaveSocket = accept(masterSocket, (struct sockaddr *) &(connectedClientAddr), &sizeClientAddr)))
	{
		inet_ntop(AF_INET, &(connectedClientAddr.sin_addr.s_addr), strClientAddr, INET_ADDRSTRLEN);
		printf(COLOR_GREEN"New client [%s] connected\n"COLOR_RESET, strClientAddr);
		
		childSlaveSocket = (int*)malloc(sizeof(int));
		*childSlaveSocket = slaveSocket;
		if( pthread_create(&mainThread, NULL, senderToClient, (void*)childSlaveSocket) < 0)
			errorMsg("could not create thread");

	}
	close(slaveSocket);
	close(masterSocket);
	return 0;
}
