#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#pragma comment(lib,"wsock32.lib")
#define MAX_LEN 1024

void InitListenSocket(SOCKET& Listen_Socket)
{
	int ret;
	WSADATA wsadata;
	int Port = 5150;
	SOCKADDR_IN Server_Addr;
	if ((ret = WSAStartup(MAKEWORD(2,2), &wsadata)) != 0)
	{
		printf("WSAStartup failed with error %d\n", ret);
		return;
	}
	if ((Listen_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{	
		printf("socket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	Server_Addr.sin_family = AF_INET;
	Server_Addr.sin_port = htons(Port);    
	Server_Addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	if (bind(Listen_Socket, (SOCKADDR *)&Server_Addr, sizeof(Server_Addr))== SOCKET_ERROR)
	{
		printf("bind failed with error %d\n", WSAGetLastError());
		closesocket(Listen_Socket);
		WSACleanup();
		return;
	}
	if (listen(Listen_Socket, 5) == SOCKET_ERROR)
	{
		printf("listen failed with error %d\n", WSAGetLastError());
		closesocket(Listen_Socket);
		WSACleanup();
		return;
	} 

	printf("We are awaiting a connection on port %d.\n", Port);
}

//数据报格式
struct Mes_pack
{
	char* ip_addr;
	char* data_buf;
	int   data_len;
};
struct Sock_data
{
	SOCKET sock;
	SOCKADDR_IN addr;
};


//线程接收函数
DWORD WINAPI Thread_Recv(LPVOID);  

void main(void)
{
	SOCKET        Connect_Socket,Listen_Socket;
	SOCKADDR_IN   Client_Addr;
	int           Client_Addr_Len;
	int           conn_num = 0;
	HANDLE        hThread;
	DWORD         dwThreadId; 
	Sock_data* param= new Sock_data;

	InitListenSocket(Listen_Socket);

	Client_Addr_Len = sizeof(SOCKADDR);
   
	int i=0;
	//CreateThread(NULL,0,Thread_Recv,(LPVOID)param,0,&dwThreadId);
	while (true)
	{
		if((param->sock = accept(Listen_Socket, (SOCKADDR *) &param->addr,&Client_Addr_Len)) != INVALID_SOCKET)
		{
			printf("connection num =  %d,socket =  %d\n",++conn_num,param->sock);
			hThread = CreateThread(NULL,0,Thread_Recv,(LPVOID)param,0,&dwThreadId);	
		}
	}
	closesocket(Connect_Socket);
	WSACleanup();
}


DWORD WINAPI Thread_Recv(LPVOID param)   
{
	Sock_data* data = new Sock_data;
	data->sock = ((Sock_data*)param)->sock;
	char   buf[256];   
	int   Ret;   
	while(true)
	{
		if((Ret=recv(data->sock,buf,256,0))>0)   
		{
			printf("recv_sock:   %d,   ",data->sock);
			printf("receive num= %d\n",Ret);
		
		}
	}
}