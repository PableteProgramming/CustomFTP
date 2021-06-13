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
				exec(message, client->GetId());
				//std::cout << message << std::endl;
				//std::cout<<"["<<client->GetName()<<"] > "<<message<<std::endl;
			}
		}
	}
}

#ifdef __linux__
void exec(std::string command, int sock) {
#else
void exec(std::string command, SOCKET sock) {
#endif
	if (command != "") {
		std::vector<std::string> temp = SplitV(command, ' ');
		std::string cmd = temp[0];
		//std::cout << cmd << std::endl;
		std::string params;
		for (int i = 1; i < temp.size(); i++) {
			params+=temp[i]+" ";
		}
		params = EndStrip(params);
		bool found = false;
		for (int i = 0; i < commands.size(); i++) {
			if (commands[i].first == cmd) {
				found = true;
				SocketSend(sock, commands[i].second(params));
			}
		}
		if (!found) {
			std::string s = "Command not found: " + cmd;
			SocketSend(sock, s);
		}
	}
}