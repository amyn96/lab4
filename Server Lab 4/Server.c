#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){

	int sockfd, ret, newSocket, port = 8888;
	struct sockaddr_in serverAddr;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[3000];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.43.132");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Error in binding.\n");
		exit(1);
	}
	printf("Bind to port %d\n", port);

	if(listen(sockfd, 10) == 0){
		printf("Waiting for Clients...\n");
	}else{
		printf("Error in binding.\n");
	}


	while(1){
		
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		
		if(newSocket < 0){
			exit(1);
		}
		
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
	
		close(sockfd);

		while(1){
			if(recv(newSocket, buffer, 3000, 0) < 0){
				printf("Error in receiving data.\n");
			}else{
				//puts(buffer);
				printf("\nClient: %s", buffer);
			}	
			
			printf("Server: ");
			fgets(buffer, 100, stdin);
			send(newSocket, buffer, strlen(buffer), 0);
				
			
		}	
		

	}

	close(newSocket);

	return 0;
}