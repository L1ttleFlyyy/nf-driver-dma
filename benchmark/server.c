//UDPServer.c

/* 
 *  gcc -o server UDPServer.c
 *  ./server
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 1024
#define SERVER_PORT 20480

void err(char *str)
{
	perror(str);
	exit(1);
}

int main(void)
{
	struct sockaddr_in my_addr, cli_addr;
	int sockfd, i;
	socklen_t slen = sizeof(cli_addr);
	char buf[BUFLEN];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err("socket");
	else
		printf("Server : socket() successful\n");

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVER_PORT);
	inet_aton("127.0.0.1", &my_addr.sin_addr);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
		err("bind");
	else
		printf("Server : bind() successful\n");

	while (1)
	{
		if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *)&cli_addr, &slen) == -1)
			err("recvfrom()");
		printf("Received packet from %s:%d\nData: %s\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);
		if (sendto(sockfd, "hello", 8, 0, (struct sockaddr *)&cli_addr, slen) == -1)
			err("sendto()");
	}

	close(sockfd);
	return 0;
}
