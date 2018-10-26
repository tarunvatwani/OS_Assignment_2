#include "globalheader.h"

extern vector<string> global_command_vector;

void read_input(){

		string command;
		string argument;
		
		cout<<"1. Share Data\n";
		cout<<"2. Download Data\n";
		cout<<"3. Remove Torrent\n";
		cout<<"4. Break\n";						
		cout<<"Enter Your Command\n";
			
		getline(cin,command);
		if(!(command.empty())){
			istringstream stream(command); 

			while(stream>>argument)
				global_command_vector.push_back(argument);
		}
		
}
