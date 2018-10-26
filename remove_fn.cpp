#include "globalheader.h"


void bin_to_hex( unsigned char * src, int len, char * hex )
{
    int i, j;

    for( i = 0, j = 0; i < len; i++, j+=2 )
        sprintf( &hex[j], "%02x", src[i] );
}


void remove(string cl_url, string tr1_url, string tr2_url, vector<string> command_vector)
{

	/* --------------------------- Checking the Validity of Command ------------------------------------ */		

	if(command_vector.size() != 2){
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
    		bin_to_hex( outbuf, sizeof(outbuf), sha_str);
	
			hash_hash_chunks = string(sha_str);
		/* ------------------------------------------------------------------------------- */
	
	char ack[3];
	send(socket_fd,"remove",strlen("remove"),0);
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}  

	send(socket_fd, hash_hash_chunks.c_str(), SHA_DIGEST_LENGTH, 0);	
  	if (recv(socket_fd, ack, 3,0) == 0){
   		perror("The server terminated prematurely");
   		return;
  	}

    send(socket_fd, filename.c_str(), filename.size(), 0);
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
