#include <client.hpp>

#ifdef __linux__
int sock=0;
#else
SOCKET sock=INVALID_SOCKET;
#endif
bool running = false;

int client(std::string ip, int port,std::string name,std::string username,std::string password){
#ifdef __linux__
	sock = 0;
	struct sockaddr_in sockInfo;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::cout<<"Socket creation error"<<std::endl;
		return 1;
	}

	sockInfo.sin_family = AF_INET;
	sockInfo.sin_port = htons(port);

	int inetResult= inet_pton(AF_INET, ip.c_str(), &sockInfo.sin_addr);
	if(inetResult<=0)
	{
		std::cout<<"Invalid address/ Address not supported"<<std::endl;
		return 1;
	}
	
	int connectResult= connect(sock, (struct sockaddr *)&sockInfo, sizeof(sockInfo));
	if (connectResult < 0)
	{
		std::cout<<"Connection Failed"<<std::endl;
		return 1;
	}
#else
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
	WSADATA wsaData;
	sock = INVALID_SOCKET;
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("Error when init WSAStartup!\n");
		exit(1);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		sock = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
#endif
	signal(SIGINT, Client_Ctrl_Handler);
	SocketSend(sock, username);
	SocketRead(sock);
	SocketSend(sock, sha512(password));
	std::string response = SocketRead(sock); //Y or N
	if (response == "N") {
		std::cout << "Username or password are incorrect !" << std::endl;
		exit(0);
	}
	else {
		std::cout << "Username and password are both correct !" << std::endl;
	}
	SocketSend(sock,name);
	std::string nameok=SocketRead(sock);
	if (nameok=="N") {
		std::cout << "Sorry, name " << name << " already in use !" << std::endl;
		return 1;
	}
	running=true;
	std::thread reading(Read, sock);
	while(running){
		std::string message;
		std::cout << "[You] > ";
		getline(std::cin,message);
		if(message=="exit"){
			running=false;
		}
		if(message!=""){
			SocketSend(sock,message);
		}
	}
	reading.join();
	return 0;
}

std::vector<std::string> GetArgs(int argc,char** argv){
	std::vector<std::string> r;
	r.clear();
	for(int i=0; i< argc; i++){
		r.push_back(std::string(argv[i]));
	}
	return r;
}

#ifdef __linux__
void Read(int s){
#else
void Read(SOCKET s) {
#endif
	while (running) {
		std::string message = SocketRead(s);
		if (message == "exit") {
			if (running) {
				running = false;
				std::cout << "Server closed" << std::endl;
				std::cout << "Press enter to exit..." << std::endl;
			}
			else {
				std::cout << "Connection closed !" << std::endl;
			}
		}
	}
}

void Client_Ctrl_Handler(int sig) {
	if (sig == 2) {
		//Ctrl C
		running = false;
		SocketSend(sock, "exit");
		std::cout << "Server connection closed successfully" << std::endl;
		exit(0);
	}
}