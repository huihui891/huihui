
#include <winsock2.h>
#include <stdio.h>
#include "string.h"
#pragma comment(lib,"wsock32.lib")

void main()
{
   WSADATA              wsaData;
   SOCKET               s;
   SOCKADDR_IN          ServerAddr;
   int                  Port = 5150;
   int                  Ret;
   char *sendbuff = new char[1024];


	for(int i=0;i<10;i++)
		sendbuff[10] = i;

   if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
   {
     printf("WSAStartup failed with error %d\n", Ret);
      return;
   }
 
   if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
       == INVALID_SOCKET)
   {
      printf("socket failed with error %d\n", WSAGetLastError());
      WSACleanup();
      return;
   }
   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);
   ServerAddr.sin_addr.s_addr = inet_addr("192.168.0.36");


   if (connect(s, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr)) 
       == SOCKET_ERROR)
   {
      printf("connect failed with error %d\n", WSAGetLastError());
      closesocket(s);
      WSACleanup();
      return;
   } 
   printf("已经连接成功!");
   while(true){
   printf("请输入你要传输的数据:\n");
   scanf("%s",sendbuff);
   printf("总共传输的字节数目:  %d\n",strlen(sendbuff));
   Ret = send(s,sendbuff,strlen(sendbuff),0);
   if(Ret<=0)
   {
	   printf("Send error");

   }
   printf("send %d\n",Ret);
   }
   closesocket(s);
   WSACleanup();
}