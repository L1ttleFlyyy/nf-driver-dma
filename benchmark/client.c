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
#include <sys/time.h>
#define BUFLEN 1024
#define SERVER_PORT 20480
#define SERVER_ADDR "10.1.0.2"
#define CLIENT_PORT 20480
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
    unsigned char buf[BUFLEN];
    unsigned char RDMA_Request[] = "Hello 456789  ";
    struct timeval start, end, diff;
    int data_len = 0;
    if(argc != 2){
	printf("Usage: client_UDP <data length>\n");
	exit(1);
    }
    data_len = atoi(argv[1]);
    if(data_len != 64 && data_len != 512 && data_len != 1024){
	data_len = 64;
    }
    RDMA_Request[10] = (unsigned char) (data_len/256);
    RDMA_Request[11] = (unsigned char) (data_len%256);
//    printf("%d, %d \n", RDMA_Request[10], RDMA_Request[11]);
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
    memcpy(buf, RDMA_Request, sizeof(RDMA_Request));
    gettimeofday(&start, NULL);
    if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&server_addr, slen) == -1)
        err("sendto()");
    if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&server_addr, &slen) == -1)
        err("recvfrom()");
    gettimeofday(&end, NULL);
    timersub(&end, &start, &diff);
    printf("Received packet from %s:%d\nData: %s\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), buf);
    printf("%ld.%03ld%03ld seconds elapsed\n", diff.tv_sec, diff.tv_usec/1000, diff.tv_usec%1000);
    close(sockfd);
    return 0;
}
