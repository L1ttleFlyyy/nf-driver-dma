//UDPClient.c

/*
 * gcc -o client UDPClient.c
 * ./client 
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFLEN 1024
#define SERVER_PORT 20480
#define SERVER_ADDR "127.0.0.1"
#define CLIENT_PORT 40960
#define CLIENT_ADDR "0.0.0.0"

void err(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr, client_addr;
    int sockfd, i, slen = sizeof(server_addr);
    char buf[BUFLEN];
    char RDMA_Request[] = "Hello     0123456789  ";
    clock_t t;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        err("socket");

    bzero(&server_addr, slen);
    bzero(&client_addr, slen);
    server_addr.sin_family = AF_INET;
    client_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    client_addr.sin_port = htons(CLIENT_PORT);
    inet_aton(SERVER_ADDR, &server_addr.sin_addr);
    inet_aton(CLIENT_ADDR, &client_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&client_addr, slen))
    {
        err("port binding failed");
    }
    strcpy(buf, RDMA_Request);
    t = clock();
    if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&server_addr, slen) == -1)
        err("sendto()");
    if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&server_addr, &slen) == -1)
        err("recvfrom()");
    t = clock() - t;
    printf("Received packet from %s:%d\nData: %s\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), buf);
    printf("%f seconds elapsed\n", ((double)t)/CLOCKS_PER_SEC);
    close(sockfd);
    return 0;
}
