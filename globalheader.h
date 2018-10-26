#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <map>

using namespace std;

#ifndef INC_GLOBALHEADER_H
#define INC_GLOBALHEADER_H

extern vector<string> global_command_vector;

void share(string, string, string, vector<string>);
void get(string, string, string, vector<string>);
void remove(string, string, string, vector<string>); 
void read_input();
void peer_server(string);
int  sha1(string, string&, string&);
int  client_init(int, string);
int  server_init(int, struct sockaddr_in&, string);

#endif
