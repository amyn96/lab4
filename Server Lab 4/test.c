#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

//fucntion to send to all client
void broadcast(char *msg,int curr){
    int i, clients[10], n = 0;
	pthread_mutex_t mutex;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < n; i++) {
        if(clients[i] != curr) {
            if(send(clients[i],msg,strlen(msg),0) < 0) {
                printf("sending failure \n");
                continue;
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

//function to receive message from client
void *recvmg(void *clientSocket){
    int sock = *((int *)clientSocket);
    char msg[500];
    int con;
    while((con = recv(sock,msg,500,0)) > 0) {
        msg[con] = '\0';
        broadcast(msg,sock);
    }

}

int main(){
	
    struct sockaddr_in ServerAddr;
    pthread_t recvt;
    int bin, sock = 0 , ClientSocket = 0, clients[20], n = 0, port = 8888;
	pthread_mutex_t mutex;

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    ServerAddr.sin_addr.s_addr = inet_addr("192.168.43.132");
    sock = socket( AF_INET , SOCK_STREAM, 0 );
	
	bin = bind(sock, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));

    if(bin < 0){
        printf("Error in binding.\n");
    }else{
        printf("Server Started\n");
		printf("Bind to port %d\n", port);
	}

    if( listen( sock ,20 ) == -1 ){
        printf("listening failed \n");
	}

    while(1){
        if( (ClientSocket = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 ){
            printf("accept failed  \n");
		}
		
        pthread_mutex_lock(&mutex);
        clients[n]= ClientSocket;
        n++;
		
        // creating a thread for each client 
        pthread_create(&recvt,NULL,(void *)recvmg,&ClientSocket);
        pthread_mutex_unlock(&mutex);
    }
    return 0; 

}