all: client server
.PHONY: all

client: client.c
	gcc client.c -o client_UDP
server: server.c
	gcc server.c -o server_UDP
clean:
	rm -f *.o server_UDP client_UDP
