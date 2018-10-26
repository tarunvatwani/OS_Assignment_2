#include "globalheader.h"

#define MAXLINE 4096

int file_download(int connfd){
	
	int n;
	char buf[MAXLINE];
	char ack[3] = "OK";
	
	if(connfd == -1){
		cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
		return -1;
    }

  	cout<<"Received request from client: "<<connfd<<"\n";
	
	if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
		cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
		return -1;
	}
	send(connfd, ack, 3, 0);
	
	FILE* fp = fopen(buf, "rb");
	memset(buf, 0, sizeof(buf));
	
	if( (n = recv(connfd, buf, 3, 0) ) < 0){
		cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	
	while(1)
    {		
        size_t len;
        len = fread(buf, 1, MAXLINE, fp);    
		send(connfd, buf, len, 0);
		memset(buf, 0, sizeof(buf));
        
        if( (n = recv(connfd, buf, 3, 0) ) < 0){
			cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
			return -1;
		}
		
		memset(buf, 0, sizeof(buf));

        if( len <= 0 )
            break;
	}
	
	send(connfd,"",0,0);
	fclose(fp);
		
	return 0;
}

void peer_server(string url){

	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in new_port;

    if(server_init(socket_fd, new_port, url) == -1){
		cout<<"Error\n";
		return;
	}
            
    while(1) {
    
    	socklen_t clilen = sizeof(new_port);
    	int connfd = accept(socket_fd, (struct sockaddr *)&new_port, &clilen);
    	
    	thread new_client(file_download,connfd);
    	new_client.detach();
 	}
 
 	close (socket_fd);
}

