#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){

	int clientSocket, ret, port = 8888;
	struct sockaddr_in serverAddr;
	char buffer[3000];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.43.132");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Connected to Server.\n");

	while(1){
		printf("Client: ");
		//scanf("%s", &buffer[0]);
		fgets(buffer, 100, stdin);
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, 3000, 0) < 0){
			printf("Error in receiving data.\n");
		}else{
			//puts(buffer);
			printf("Server: %s\n", buffer);
		}
	}

	return 0;
}