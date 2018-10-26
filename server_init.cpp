#include "globalheader.h"

int server_init(int socket_fd, struct sockaddr_in& new_port, string url){
	int opt = 1;
	string port = url.substr(url.find_first_of(':')+1);

	if(socket_fd == -1){
		cout<<"Error in initalizing socket line -- :"<<strerror(errno);
	}
	
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
		cout<<"Setsocket opt-- :"<<strerror(errno)<<"\n";        
        return -1; 
    } 

	new_port.sin_family = AF_INET;
	new_port.sin_port = htons(static_cast<uint16_t>(stoi(port)));
	new_port.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(socket_fd, (struct sockaddr *)&new_port, sizeof(new_port)) == -1){
		cout<<"Error in binding -- :"<<strerror(errno)<<"\n";
		return -1; 
	}
	
	if (listen(socket_fd, 10) == -1) 
    { 
		cout<<"Error in listening -- :"<<strerror(errno)<<"\n";
		return -1;
    } 
	
	return 0;
}

