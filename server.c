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
	int server_fd;
	
	int new_socket;
	int client_sockets[CONFIG_MAX_CLIENTS];
	memset(client_sockets, 0, sizeof(client_sockets));

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	msg_pak_t user_packet;
	fd_set readfds;

	msg_pak_t packet_buffer;

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
		
		for (int i = 0; i < CONFIG_MAX_CLIENTS; ++i) {
			int fd = client_sockets[i];
			if(!fd) break;

			FD_SET(fd, &readfds);
			if(fd > max_fd) max_fd = fd;
		} 
	
		select(max_fd + 1, &readfds, NULL, NULL, NULL);
		
		if (FD_ISSET(server_fd, &readfds)) {
			new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
			printf("New connection from %s [FD: %d]\n", inet_ntoa(address.sin_addr), new_socket);
			for (int i = 0; i < CONFIG_MAX_CLIENTS; ++i)
				if (client_sockets[i] == 0) {
					client_sockets[i] = new_socket;
					break;
				}

		}
		
		for (int i = 0; i < CONFIG_MAX_CLIENTS; ++i) {
			int fd = client_sockets[i];
			if (FD_ISSET(fd, &readfds)) {
				int val = read(fd, &packet_buffer, sizeof(packet_buffer));
				if (!val) {
					getpeername(fd, (struct sockaddr *)&address, &addrlen);
					printf("Client %s disconnected\n", inet_ntoa(address.sin_addr));
					close(fd);
					client_sockets[i] = 0;	
					continue;
				}

				for (int j = 0; j < CONFIG_MAX_CLIENTS; ++j) {
					if (client_sockets[j] != 0 && client_sockets[j] != fd) {
						send(client_sockets[j], &packet_buffer, sizeof(packet_buffer), 0);	
					}
				}
				printf("%s: %s\n", packet_buffer.sender, packet_buffer.msg);
			}
		}
	}		

	return EXIT_SUCCESS;
}
