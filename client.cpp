#include "globalheader.h"


int main(int argc, char *argv[]){
	
	if(argc != 5){
		cout<<"Error:Insufficient Arguments to script\n";
		return -1;
	}

	void (*peer_server_ptr)(string) = &peer_server;
	thread peer_server_th(peer_server_ptr, string(argv[1]));
	peer_server_th.detach();		
	
	while (true){
	
		read_input();
		
		if(global_command_vector[0] == "share"){			
			void (*share_ptr)(string, string, string, vector<string>) = &share;
			thread share_th(share_ptr, string(argv[1]), string(argv[2]), string(argv[3]), global_command_vector);
			share_th.detach();
			global_command_vector.clear();
		}
		
		else if(global_command_vector[0] == "get"){
			void (*get_ptr)(string, string, string, vector<string>) = &get; 
			thread get_th(get_ptr, string(argv[1]), string(argv[2]), string(argv[3]), global_command_vector);
			get_th.detach();
			global_command_vector.clear();
		}

		else if(global_command_vector[0] == "remove"){
			void (*remove_ptr)(string, string, string, vector<string>) = &remove;
			thread remove_th(remove_ptr, string(argv[1]), string(argv[2]), string(argv[3]), global_command_vector);
			remove_th.detach();
			global_command_vector.clear();
		}

		else if(global_command_vector[0] == "break"){
			break;
		}
		
		else{
			global_command_vector.clear();
			cout<<"Invalid Command !!!\n";
		}
		
	}
	
	return 0;
}
