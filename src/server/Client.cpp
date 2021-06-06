#include <Client.hpp>

#ifdef __linux__
Client::Client(int _id, std::string _name){
#else
Client::Client(SOCKET _id, std::string _name) {
#endif
	id=_id;
	name=_name;
	exit=false;
	reading=NULL;
}

void Client::StartThread(){
	reading= new std::thread(Read,this);
}

std::string Client::GetName(){
	return name;
}

#ifdef __linux__
int Client::GetId(){
#else
SOCKET Client::GetId() {
#endif
	return id;
}

void Client::JoinThread(){
	reading->join();
}

void Read(Client* client){
	while(!client->exit){
		std::string message= SocketRead(client->GetId());
		if(message=="exit"){
			client->exit=true;
			SocketSend(client->GetId(), "exit");
		}
		else{
			if(message!=""){
				exec(message);
				//std::cout<<"["<<client->GetName()<<"] > "<<message<<std::endl;
			}
		}
	}
}

void exec(std::string command) {
	if (command != "") {
		std::vector<std::string> temp = SplitV(command, ' ');
		std::string cmd = temp[0];
		std::vector<std::string> params;
		for (int i = 1; i < temp.size(); i++) {
			params.push_back(temp[i]);
		}

		if (cmd == "ls") {
			ls(params);
		}
	}
}