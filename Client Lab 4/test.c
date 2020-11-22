#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>


void *recvmg(void *my_sock)
{
    int sock = *((int *)my_sock);
    int con;
	char msg[500];
	
    //client always ready to receive message
    while((con = recv(sock,msg,500,0)) > 0) {
        msg[con] = '\0';
        fputs(msg,stdout);
    }
}

int main(int argc, char *argv[]){
    pthread_t recvt;
    int ret, con, sock, port = 8888;
    char send_msg[500], client_name[100], msg[500];
    struct sockaddr_in serverAddr;
	
    strcpy(client_name, argv[0]);
    sock = socket( AF_INET, SOCK_STREAM,0);
    serverAddr.sin_port = htons(port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.43.132");
	
    /*if( (connect( sock ,(struct sockaddr *)&ServerIp,sizeof(ServerIp))) == -1 ){
        printf("\n connection to socket failed \n");
	}*/
	
	ret = connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Error in connection.\n");
		exit(1);
	}else{
		printf("Connected to Server.\n");
	}

    //creating a client which is always waiting for a message
    pthread_create(&recvt,NULL,(void *)recvmg,&sock);

    //ready to read a message from console
    while(fgets(msg,500,stdin) > 0) {
        strcpy(send_msg,client_name);
        strcat(send_msg,":");
        strcat(send_msg,msg);
        con = write(sock,send_msg,strlen(send_msg));
        if(con < 0) {
            printf("\n message not sent \n");
		}
    }

    pthread_join(recvt,NULL);
    close(sock);
    return 0;
}