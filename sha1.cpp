#include "globalheader.h"

#define chunk_size 512000

void bin2hex( unsigned char * src, int len, char * hex )
{
    int i, j;

    for( i = 0, j = 0; i < len; i++, j+=2 )
        sprintf( &hex[j], "%02x", src[i] );
}

int sha1( string filename, string& hash_chunks,  string& hash_hash_chunks)
{
	FILE *in;
    SHA_CTX ctxt;
    
    unsigned char outbuf[ SHA_DIGEST_LENGTH ];
    char sha_str[ (SHA_DIGEST_LENGTH * 2)]; 
    
    unsigned char buf[ chunk_size ];

	if( (in = fopen(filename.c_str(),"rb"))  == NULL){
		cout<<"Error in share_fn line 24 for "<<filename<<":"<<strerror(errno);
		return -1;
	}

	
    SHA1_Init( &ctxt );
		
	while(1)
    {
        size_t len;
		
        len = fread( buf, 1, chunk_size, in);
        
        if( len <= 0 )
            break;
		
		SHA1(buf, len, outbuf);
        SHA1_Update( &ctxt, buf, len );
        SHA1_Final( outbuf, &ctxt );
		
        bin2hex( outbuf, sizeof(outbuf), sha_str );
        hash_chunks.append(string(sha_str).substr(0,20));      
    }
	
	SHA1((unsigned char *)(hash_chunks.c_str()), sizeof(hash_chunks), outbuf);
    bin2hex( outbuf, sizeof(outbuf), sha_str);
	
	hash_hash_chunks = string(sha_str);
	
    fclose(in);

    return 0;
}
