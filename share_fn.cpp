#include "globalheader.h"

void share(string cl_url, string tr1_url, string tr2_url, vector<string>command_vector)
{

	/* --------------------------- Checking the Validity of Command ------------------------------------ */		

	if(command_vector.size() != 3){
		cout<<command_vector.size()<<"\n";
        printf("Invalid Arguments!\n");
        return;
	}

    FILE *out;
	string hash_chunks, hash_hash_chunks;

	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);

	/* --------------------------- Calculating Hash of file  ------------------------------------ */		
	
	if( sha1( command_vector[1], hash_chunks, hash_hash_chunks) )
    {
        printf("Error!\n");
        return;
    }

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

	/* --------------------------- Writing in Mtorrent File ------------------------------------ */		
    
	if( (out = fopen(command_vector[2].c_str(),"wb"))  == NULL){
		cout<<"Error in share_fn line 37 for "<<command_vector[2]<<":"<<strerror(errno);
		return ;
	}
	
	fprintf(out, "%s", tr1_url.c_str());
	fprintf(out, "\n");

	fprintf(out, "%s", tr2_url.c_str());	
	fprintf(out, "\n");
	
	fprintf(out, "%s\n", realpath(command_vector[1].c_str(),NULL));
	
	struct stat filestatus;
  	stat( realpath(command_vector[1].c_str(),NULL), &filestatus );

	fprintf(out,"%zu\n", filestatus.st_size);
    fwrite (hash_chunks.c_str(), 1, hash_chunks.size(), out);


	/* --------------------------- Sending Data to Tracker Server ------------------------------------ */
	
	char ack[3];
	send(socket_fd,"share",strlen("share"),0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}  

	send(socket_fd, hash_hash_chunks.c_str(), SHA_DIGEST_LENGTH, 0);	
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}

    send(socket_fd, realpath(command_vector[1].c_str(),NULL), strlen(realpath(command_vector[1].c_str(),NULL)), 0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}
  	
    send(socket_fd, cl_url.c_str(), cl_url.size(), 0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}
  	  
	fclose(out);
}
