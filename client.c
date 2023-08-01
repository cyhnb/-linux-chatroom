#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

// ./client
/* socket
 * bind
 * connect
 * send/recv
 * close
 */
#define port 8080

static char buf[128];
int main(int argc, char **argv)
{
	int ret;
	int socketClient;
	struct sockaddr_in addrServer;
	char server_addr[20] = {0};
	
	memset(&addrServer, 0x0, sizeof(addrServer)); //清零
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(port);
	ret = inet_pton(AF_INET, "127.0.0.1", &addrServer.sin_addr);
	if(ret <= 0)
	{
		perror("inet_pton error\r\n");
 		exit(-1);
	}
	/* 1. 判断参数 */
	if (argc != 1) 
	{
		printf("Usage: %s <string>\r\n", argv[0]);
		return -1;
	}
	//socket
	socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if ( socketClient < 0) {
 		perror("socket error\r\n");
 		exit(-1);
	}

	//connect
	ret =  connect(socketClient,(const struct sockaddr*)&addrServer, sizeof(struct sockaddr));
	if ( ret < 0) {
 		perror("connect error\r\n");
 		exit(-1);
	}
	inet_ntop(AF_INET, &addrServer.sin_addr.s_addr,server_addr, sizeof(server_addr));
	printf("connect server_addr:%s succeed\r\n",server_addr);
	//send
	while(1)
	{
		sprintf(buf, "%d", getpid());
		ret = send(socketClient, buf, sizeof(buf), 0);
		if (ret < 0) {
 			perror("send error\r\n");
 			exit(-1);
		}
		printf("send ret is %d,buf is %s\r\n",ret,buf);
		sleep(1);
	}
	//close
	close(socketClient);
	
	return 0;
}




