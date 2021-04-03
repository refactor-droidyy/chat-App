/*

file_name server_ipadress portno
argv[0] filename
argv[1] server_ipadress
argv[2] portno

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(int argc,char *argv[]){
	
	int sockfd,portno,n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if(argc < 3){
		fprintf(stderr,"usage %s hotname port\n",argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	if(sockfd < 0){
		error("Error opening socket");
	}

	server = gethostbyname(argv[1]);
	if(server == NULL){
		fprintf(stderr,"no Such host");
        exit(0);
	}
	bzero((char *) &serv_addr,sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0){
		error("Connection Failed !!");
	}
    printf("Clinet:");
	while(1){
		bzero(buffer,256);
		fgets(buffer,256,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if(n < 0){
			error("Error on Writing to socket");
		}

		bzero(buffer,256);

		n = read(sockfd,buffer,255);
		if(n < 0){
			error("Error On Reading");
		}
		printf("Server : %s\n",buffer);

		int i = strncmp("Over",buffer,4);
		if(i == 0){
			break;
        }
	}

	close(sockfd);
	return 0;

}
