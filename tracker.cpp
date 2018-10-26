#include "globalheader.h"

#define MAXLINE 4096

using namespace std;

map <string, vector<pair <string,string> > >seederlist_map;

int receive_data(int connfd, string seederlist){
	
	FILE *fp;
	int n;
	char buf[MAXLINE];
	char ack[3] = "OK";
	bool found = 0;

	if(connfd == -1){
		cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
		return -1;
    }

  	cout<<"Received request from client: "<<connfd<<""<<"\n";

	/*---------------------------- Checking First Message ------------------------------------------*/
	
	if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
		cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
		return -1;
	}
	send(connfd, ack, 3, 0);

   	string test(buf);
 	memset(buf, 0, sizeof buf);

	/*---------------------------- Share Functionality of Tracker ------------------------------------------*/ 	
 	
 	if(test.compare("share") == 0 ){
 		vector<string> temp(3,"\0");
 		for(int i=0; i<3; i++){
			if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
			}
			send(connfd, ack, 3, 0);
			temp[i] = string(buf);
			memset(buf, 0, sizeof buf);
 		}
 		
 		if((fp = fopen(seederlist.c_str(),"a")) == NULL){
 			cout<<"Error in seederlist"<<strerror(errno);
 		}

		auto itr = seederlist_map[temp[0]].begin();		
		while(itr < seederlist_map[temp[0]].end()){
		
			if( (itr->first == temp[1]) && (itr->second == temp[2]))
			{
				found = 1;
				break;
			}
			itr++;				
		}
		
		if(found == 0){
 			fprintf(fp,"%s",temp[0].c_str());
 			fprintf(fp," ");
 			fprintf(fp,"%s",temp[1].c_str());
 			fprintf(fp," ");
 			fprintf(fp,"%s",temp[2].c_str());
 			fprintf(fp,"\n");
 			fclose(fp);
 		
 			seederlist_map[temp[0]].push_back(make_pair(temp[1],temp[2]));
 			found = 0;
		}
 	}
 	

	/*---------------------------- Get Functionality of Tracker ------------------------------------------*/ 	
 	
 	if(test.compare("get") == 0 ){
 		if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
			cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
			return -1;
		}
		send(connfd, ack, 3, 0);
		string temp_hash(buf);
		memset(buf, 0, sizeof buf);

		if( (n = recv(connfd, buf, 3, 0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
		}
		
		int size = seederlist_map[temp_hash].size();
		send(connfd, &(size), sizeof(size), 0);
		memset(buf, 0, sizeof buf);
		
		int d = seederlist_map[temp_hash].size();
		for(int i=0; i<d; i++){			
			if( (n = recv(connfd, buf, 3, 0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
			}
			send(connfd, seederlist_map[temp_hash][i].first.c_str(), seederlist_map[temp_hash][i].first.size(), 0);
			memset(buf, 0, sizeof(buf));
			
			if( (n = recv(connfd, buf, 3, 0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
			}
			send(connfd, seederlist_map[temp_hash][i].second.c_str(), seederlist_map[temp_hash][i].second.size(), 0);
			memset(buf, 0, sizeof(buf));
		}
		
	 	vector<string> temp(3,"\0");
 		for(int i=0; i<3; i++){
			if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
			}
			send(connfd, ack, 3, 0);
			temp[i] = string(buf);
			memset(buf, 0, sizeof buf);
 		}
 		
 		if((fp = fopen(seederlist.c_str(),"a")) == NULL){
 			cout<<"Error in seederlist"<<strerror(errno);
 		}

		auto itr = seederlist_map[temp[0]].begin();		
		while(itr < seederlist_map[temp[0]].end()){
		
			if( (itr->first == temp[1]) && (itr->second == temp[2]))
			{
				found = 1;
				break;
			}
			itr++;				
		}
		
		if(found == 0){
 			fprintf(fp,"%s",temp[0].c_str());
 			fprintf(fp," ");
 			fprintf(fp,"%s",temp[1].c_str());
 			fprintf(fp," ");
 			fprintf(fp,"%s",temp[2].c_str());
 			fprintf(fp,"\n");
 			fclose(fp);
 		
 			seederlist_map[temp[0]].push_back(make_pair(temp[1],temp[2]));
 			found = 0;
		}

 	}

	/*---------------------------- Remove Functionality of Tracker ------------------------------------------*/ 	
 	
 	if(test.compare("remove") == 0 ){
 		vector<string> temp(3,"\0");
 		for(int i=0; i<3; i++){
			if( (n = recv(connfd, buf, MAXLINE,0) ) < 0){
				cout<<"Error in estblishing connection -- :"<<strerror(errno)<<"\n";
				return -1;
			}
			send(connfd, ack, 3, 0);
			temp[i] = string(buf);
			memset(buf, 0, sizeof buf);
 		}
		
		auto itr = seederlist_map[temp[0]].begin();
		while(itr < seederlist_map[temp[0]].end()){
			if( (itr->first == temp[1]) && (itr->second == temp[2]))
			{
				seederlist_map[temp[0]].erase(itr);
				
				string deleteline = temp[0] + " " + temp[1] + " " + temp[2];
				string line;

				ifstream fin;
				fin.open(seederlist.c_str());
				ofstream temp;
				temp.open("temp.txt");
				
				while (getline(fin,line))
				{
					if (line != deleteline){
        				temp << line << "\n";
					} 
				}				
				
				temp.close();
				fin.close();
				remove(seederlist.c_str());
				rename("temp.txt",seederlist.c_str());
				
				break;
			}
			itr++;				
		}
 	}	
	 	
 	memset(buf, 0, sizeof buf); 
 	close(connfd);
 	return 0;
}


int main(int argc, char *argv[]){

	if(argc != 5){
		cout<<"Error:Insufficient Arguments to script\n";
		return -1;
	}
	
	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in new_port;

    if(server_init(socket_fd, new_port, string(argv[1])) == -1){
		cout<<"Error\n";
		return -1;
	}
    
    ifstream fin;
	fin.open(argv[3]);
	string line;
				
	while (getline(fin,line))
	{
		istringstream iss(line);
		string hash_hash_chunks, filename, cl_url;
		if (!( iss >> hash_hash_chunks >> filename >> cl_url )) { break; }
		seederlist_map[hash_hash_chunks].push_back(make_pair(filename,cl_url));
	}				
    
    fin.close();
        
    while(1) {
    
    	socklen_t clilen = sizeof(new_port);
    	int connfd = accept(socket_fd, (struct sockaddr *)&new_port, &clilen);
    	
    	thread new_client(receive_data,connfd,string(argv[3]));
    	new_client.detach();
 	}
 
 	close (socket_fd);
    return 0;
}
