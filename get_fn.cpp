#include "globalheader.h"

#define MAXLINE 4096

void bin_2_hex( unsigned char * src, int len, char * hex )
{
    int i, j;

    for( i = 0, j = 0; i < len; i++, j+=2 )
        sprintf( &hex[j], "%02x", src[i] );
}


int peer_client(int socket_fd, string filename, string url, string local_filename){
    
    int n;
    char ack[3];
    char buf[MAXLINE];
    
    if(client_init(socket_fd, url) < 0){
    	cout<<"Error\n";
    	return -1;
    }

	send(socket_fd, filename.c_str(), filename.size(), 0);	
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return -1;
  	}
  	
	ofstream fp;
	fp.open(local_filename, ios::out | ios::binary);	
  	
  	send(socket_fd,"OK",strlen("OK"),0);
	n = recv(socket_fd, buf, MAXLINE,0);
	fp.write(buf,n);
	memset(buf, 0, sizeof(buf));
	
	while ( n > 0){
		send(socket_fd, "OK",strlen("OK"), 0);
		n = recv(socket_fd, buf, MAXLINE,0);
		fp.write(buf,n);
		memset(buf, 0, sizeof(buf));
  	}
  	
  	fp.close();
  	
  	cout<<"File Transfer Complete\n";
  	
  	if (recv(socket_fd, buf, MAXLINE, 0) < 0){
   		perror("The server terminated prematurely");
   		return -1;
  	}
 	  	
  	return 0;

}

void get(string cl_url, string tr1_url, string tr2_url, vector<string> command_vector)
{

	/* --------------------------- Checking the Validity of Command ------------------------------------ */		

	if(command_vector.size() != 3){
        printf("Invalid Arguments!\n");
        return;
	}

	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	/* --------------------------- Initializing Client Connection ------------------------------------ */		
    
    int x = client_init(socket_fd, tr1_url);
    if(x < 0){
		cout<<"Error in Connecting Tracker 1\n";

		if(x == -2){
			if(client_init(socket_fd, tr2_url) < 0)
				cout<<"Error in Connecting Tracker 2\n";
		}

		return;
	}

	/* --------------------------- Reading from Mtorrent File ------------------------------------ */		
    
	ifstream infile(command_vector[1]);
	string line;
	string filename, hash_chunks;
	
	int count = 1;
	while (getline(infile, line))
	{
		if(count == 3){
			filename = line;
			count++;
		}
		else if(count == 5){
			hash_chunks = line;
			count++;
		}
		else{
			count++;
			continue;
		}
	}
	
	infile.close();	

	/* --------------------------- Sending Data to Tracker Server ------------------------------------ */
	
		/* -----------------------	Calculating Hash of Hash ---------------------------- */	
			unsigned char outbuf[ SHA_DIGEST_LENGTH ];
    		char sha_str[ (SHA_DIGEST_LENGTH * 2)]; 
			string hash_hash_chunks;			
	
			SHA1((unsigned char *)(hash_chunks.c_str()), sizeof(hash_chunks), outbuf);
    		bin_2_hex( outbuf, sizeof(outbuf), sha_str);
	
			hash_hash_chunks = string(sha_str);
		/* ------------------------------------------------------------------------------- */
	
	char ack[3];
	char buf[MAXLINE];
  	memset(buf, 0, sizeof(buf));
	int d;
	vector<pair<string,string>> client_list;
	
	send(socket_fd,"get",strlen("get"),0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}  

	send(socket_fd, hash_hash_chunks.c_str(), SHA_DIGEST_LENGTH, 0);	
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}

	send(socket_fd,"OK",strlen("OK"),0);
	if (recv(socket_fd, &d, sizeof(d),0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}
  	  	
  	for(int i=0; i<d; i++){
  		send(socket_fd,"OK",strlen("OK"),0);
		if (recv(socket_fd, buf, sizeof(buf),0) == 0){
   			perror("The server terminated prematurely");
   			return;
  		}
  		string temp1(buf);
  		memset(buf, 0, sizeof(buf));

  		send(socket_fd,"OK",strlen("OK"),0);
		if (recv(socket_fd, buf, sizeof(buf),0) == 0){
   			perror("The server terminated prematurely");
   			return;
  		}
  		string temp2(buf);
  		memset(buf, 0, sizeof(buf));
  		
  		client_list.push_back(make_pair(temp1,temp2));
  	}

  	for(int i=0; i<d; i++){
		int socket_id = socket(PF_INET, SOCK_STREAM, 0);
    	thread new_client(peer_client, socket_id, client_list[i].first, client_list[i].second, command_vector[2]);
    	new_client.detach();
  	}
  	
	send(socket_fd, hash_hash_chunks.c_str(), SHA_DIGEST_LENGTH, 0);	
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}
	
	FILE* fp = fopen(command_vector[2].c_str(), "wb");
	fclose(fp);

    send(socket_fd, realpath(command_vector[2].c_str(),NULL), strlen(realpath(command_vector[2].c_str(),NULL)), 0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}
  	
    send(socket_fd, cl_url.c_str(), cl_url.size(), 0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}

  	
}
