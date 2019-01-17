#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#endif

#include <unistd.h>    //write

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "server.h"

#pragma pack(1)
#pragma pack()


int createSocket(int port)
{
    int sock, err;
    struct sockaddr_in server;

#ifdef __WIN32__
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
#endif


    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("ERROR: Socket creation failed\n %d",sock);
        exit(1);
    }
    printf("Socket created\n");

    memset((char *) &server,0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("ERROR: Bind failed\n");
        exit(1);
    }
    printf("Bind done\n");

    listen(sock , 3);

    return sock;
}

void closeSocket(int sock)
{
    close(sock);
    return;
}

void sendMsg(int sock, void* msg, uint32_t msgsize)
{
    if (send(sock, msg, msgsize,0) < 0)
    {
        printf("Can't send message.\n");
        closeSocket(sock);
        exit(1);
    }
    printf("Message sent (%d bytes).\n", msgsize);
    return;
}

/*
 * This function prints map in plain text format
 * */
void print_map(uint16_t map[FIELD_WIDTH][FIELD_HEIGHT]){
  for(int y=0; y<FIELD_HEIGHT; y++){
    for(int x=0; x<FIELD_WIDTH; x++){
      printf("%d ",map[x][y]);
    }
    printf("\n");
  }
}

/*
 * This function prints the snake coordinates
 * The head of snake is the last coordinate
 * */
void print_snake(uint16_t snake_len, uint16_t pos[MAX_SNAKE_LEN][2]){
  for(int i=0; i<snake_len; i++){
    printf("[%d, %d]\n",pos[i][0], pos[i][1]);
  }
}


int main(int argc, char *argv[])
{
    int PORT = 2300;
    if(argc>1)
      PORT = atoi(argv[1]);
    int BUFFSIZE = 10000;
    char buff[BUFFSIZE];
    int ssock, csock;
    int nread;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    srand(time(0));
    ssock = createSocket(PORT);
    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        csock = accept(ssock, (struct sockaddr *)&client, &clilen);
        if (csock < 0)
        {
            printf("Error: accept() failed\n");
            continue;
        }

        printf("Accepted connection from %s\n", inet_ntoa(client.sin_addr));
        memset(buff,0, BUFFSIZE);
        while ((nread=recv(csock, buff, BUFFSIZE,0)) > 0)
        {
            printf("\nReceived %d bytes\n", nread);
            payload *p = (payload*) buff;
            print_map(p->map);
            reply rep = get_reply(p);
            //printf("Sending reply.. %d\n",rep.direction);

            sendMsg(csock, &rep, sizeof(reply));
        }
        printf("Closing connection to client\n");
        printf("----------------------------\n");
        closeSocket(csock);
    }

    closeSocket(ssock);
    printf("bye");
    return 0;
}
