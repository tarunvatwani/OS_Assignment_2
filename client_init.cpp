#include "globalheader.h"

int client_init(int socket_fd, string url){

	string ip = url.substr(0,url.find_first_of(':'));
	string port = url.substr(url.find_first_of(':')+1);

	if(socket_fd == -1){
		cout<<"Error in initalizing socket line -- :"<<strerror(errno)<<"\n";
		return -1;
	}
	
	struct sockaddr_in new_port;
	new_port.sin_family = AF_INET;
	new_port.sin_port = htons(static_cast<uint16_t>(stoi(port)));
	
	if(inet_pton(AF_INET, ip.c_str(), &new_port.sin_addr)<=0)  
    { 
		cout<<"Invalid address-- :"<<strerror(errno)<<"\n";        
        return -1; 
    }
        
	if( connect(socket_fd, (struct sockaddr *)&new_port, sizeof(new_port)) == -1 ){
		cout<<"Error in connect -- :"<<strerror(errno)<<"\n";	
        return -2; 		
	}
	
	return 0;
}

