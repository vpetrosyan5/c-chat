#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "config.h"
#include "shared.h"

int self_socket = 0;
char username[10];

void *receive_msg(void *arg) {
	msg_pak_t buffer;

	while(true) {
		if (read(self_socket, &buffer, sizeof(msg_pak_t))) {
			char *time_str = ctime(&buffer.timestamp);
			time_str[strcspn(time_str, "\n")] = 0;

			printf("\r      %s (%s) #\t\t%s\n", buffer.sender, time_str, buffer.msg);
			printf("\r[YOU] %s\t\t\t\t#\t", username);
			fflush(stdout);
		}
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: cchat_client <ip>\n");
		return EXIT_FAILURE;
	}

	struct sockaddr_in server_address;

	char msg[512];

	pthread_t recv_thread;	
	
	printf("Enter username: ");
	scanf("%s", username);

	self_socket = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(CONFIG_PORT);
	inet_pton(AF_INET, argv[1], &server_address.sin_addr);

	if(connect(self_socket, (struct sockaddr *)&server_address, sizeof(server_address))) {
		perror("failed to connect to server");
		return EXIT_FAILURE;
	}

	printf("Connected to server on %s:%d\n", inet_ntoa(server_address.sin_addr), CONFIG_PORT);	

	pthread_create(&recv_thread, NULL, receive_msg, NULL); 	

	while (true) {
		printf("\r[YOU] %s\t\t\t\t#\t", username);
		fgets(msg, sizeof(msg), stdin);
		
		char *msgleft = msg + strlen(msg) - 1;
		while(*msgleft == '\n' || *msgleft == '\t' || *msgleft == ' ') *(msgleft--) = 0;

		char *msgtrimmed = msg;
		while(*msgtrimmed == '\t' || *msgtrimmed == ' ') ++msgtrimmed;

		if(strlen(msgtrimmed) == 0) continue;	

		msg_pak_t outgoing_packet;
		strncpy((char *)outgoing_packet.sender, username, 10);
		strncpy((char *)outgoing_packet.msg, msgtrimmed, 512);
		outgoing_packet.timestamp = time(NULL);	

		send(self_socket, &outgoing_packet, sizeof(msg_pak_t), 0); 
	}

	return 0;
}
