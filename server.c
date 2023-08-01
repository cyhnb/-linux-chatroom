#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

// ./server

//多线程后补充
/* socket
 * bind
 * listen
 * accept
 * send/recv
 * close
 */
#define port 8080
static int clientNum = 0;
//用户数据结构体
static struct ClientData
{	
	int clientSocket;
	struct  sockaddr_in clientAddr;
	char clientip[20]; // 用于存储 IP 地址字符串的缓冲区
	char buf[128];
}*clients[10];

//服务器与用户的通信线程
static void *client_thread1(void *arg)
{	
	int ret;
	int i;
	struct ClientData clientData= *(struct ClientData*)arg;
	printf("新线程: 进程 ID<%d> 线程 ID<%lu>\n", getpid(), pthread_self());

	inet_ntop(AF_INET, &clientData.clientAddr.sin_addr.s_addr, clientData.clientip, sizeof(clientData.clientAddr));
	printf("connect clientAddr:%s suicceed\r\n",clientData.clientip);

	//分离线程
	ret = pthread_detach(pthread_self());
 	if (ret)
 	{
 		fprintf(stderr, "Error details: %s\n", strerror(ret));
 		exit(-1);
 	}
 
	//recv
	while(1)
	{
		ret = recv(clientData.clientSocket, clientData.buf, sizeof(clientData.buf), 0);
		if (ret < 0) {
 			perror("recv error\r\n");
 			exit(-1);
		}
		if (ret == 0)
		{
			printf("clientSocket:%d has closed\r\n",clientData.clientSocket);
			printf("tid :%lu end\r\n",pthread_self());
			//关闭socket
			close(clientData.clientSocket);
			//线程退出
			pthread_exit(NULL);
		}
		printf("recv ret is %d,buf is %s\r\n",ret,clientData.buf);
		for(i=0; i<clientNum; i++)
		{
			ret = send(clientData.clientSocket, "has recve", 10, 0);
			if (ret < 0) {
	 			perror("send error\r\n");
	 			exit(-1);
 			}
		}
	}
	
}

int main(int argc, char **argv)
{
	int ret;
	pthread_t tidClient; // 声明变量 tid1
	
	int serverSocket;
	struct  sockaddr_in serverAddr;
	//socklen_t addrlen;
	
	memset(&serverAddr, 0x0, sizeof(serverAddr)); //清零
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	/* 1. 判断参数 */
	if (argc != 1) 
	{
		printf("Usage: %s <string>\r\n", argv[0]);
		return -1;
	}
	//socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( serverSocket < 0) {
 		perror("socket error\r\n");
 		exit(-1);
	}
	//bind
	ret = bind(serverSocket,(const struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if ( ret < 0) {
 		perror("bind error\r\n");
 		exit(-1);
	}
	//listen
	ret = listen(serverSocket, 10);
	if ( ret < 0) {
 		perror("listen error\r\n");
 		exit(-1);
	}
	//接收clients
	while (1) {
    	int clientSocket;
   		struct sockaddr_in clientAddr;
   		socklen_t client_addrlen = sizeof(clientAddr);
		
	    // 等待客户端连接请求
	    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &client_addrlen);
	    if (clientSocket < 0) 
	    {
	        perror("accept error\r\n");
	        // 处理连接错误，继续下一个连接请求
	        continue;
	    }
		//组建client线程信息
		
		struct ClientData* clientData = (struct ClientData*)malloc(sizeof(struct ClientData)); 
		memset(clientData, 0x0, sizeof(struct ClientData)); //清零
		clientData->clientAddr = clientAddr;
		memcpy(&clientData->clientSocket, &clientSocket, sizeof(clientSocket));	
		memcpy(&clients[clientNum++], &clientData, sizeof(struct ClientData));
	    // 创建新的线程或使用其他的方法来处理与客户端的通信 
	    ret = pthread_create(&tidClient, NULL, client_thread1, clientData);
		if(ret)
		{
			perror("pthread1_create err\r\n");
			exit(-1);
		}
		
	}
	
	//close
	close(serverSocket);
	return 0;
}




