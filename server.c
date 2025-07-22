#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "shared.h"
#include "config.h"

int main(int argc, char *argv[]) {
	int server_fd;;
	
	int new_socket;
	int client_sockets[CONFIG_MAX_CLIENTS];
	memset(client_sockets, 0, sizeof(client_sockets));

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	msg_pak_t user_packet;
	fd_set readfds;

	// Create a socket for the server 
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (!server_fd) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	printf("Socket file descriptor: %d\n", server_fd);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;	
	address.sin_port = htons(CONFIG_PORT);		

	bind(server_fd, (struct sockaddr *)&address, sizeof(address));	

	listen(server_fd, 5);

	printf("Server started on port %d\n", CONFIG_PORT);

	while (true) {
		FD_ZERO(&readfds);	
		FD_SET(server_fd, &readfds);
		
		int max_fd = server_fd;			
	}		

	return EXIT_SUCCESS;
}
