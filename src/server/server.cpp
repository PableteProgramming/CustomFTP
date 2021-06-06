#include <server.hpp>
std::vector<Client*> clients;

#ifdef __linux__
int sock = 0;
#else
SOCKET sock = INVALID_SOCKET;
#endif
bool running = false;

int server(int port){

	std::string username;
	std::string password;
	std::ifstream credentials("Credentials.txt");

	if (!credentials.good())
	{
		while (true)
		{
			std::cout << "Input username : ";
			bool temp = true;
			std::getline(std::cin, username);
			for (int i = 0; i < username.size(); i++)
			{
				if (username[i] == ':')
				{
					std::cout << "Username cannot contain ':' character." << std::endl;
					temp = false;
					break;
				}
				else if (username[i] == ' ')
				{
					std::cout << "Username cannot contain space character." << std::endl;
					temp = false;
					break;
				}
			}
			if (temp == true)
			{
				break;
			}
		}
		while (true)
		{
			std::cout << "Input password : ";
			bool temp = true;
			std::getline(std::cin, password);
			for (int i = 0; i < password.size(); i++)
			{
				if (password[i] == ' ')
				{
					std::cout << "Password cannot contain space character." << std::endl;
					temp = false;
					break;
				}
			}
			if (temp == true)
			{
				break;
			}
		}

		credentials.close();

		std::ofstream writeFile("Credentials.txt");
		password = sha512(password);
		writeFile << username << ":" << password;
		writeFile.close();
	}
	else
	{
		std::string content;
		std::getline(credentials, content);

		std::pair<std::string, std::string> r = Split(content, ':');

		if (!r.first.empty() && r.second.size() == 128)
		{
			username = r.first;
			//std::cout << "Username : " << username << std::endl;
			password = r.second;
			//std::cout << "Password : " << password << std::endl;
			credentials.close();
		}
		else
		{
			credentials.close();
			remove("Credentials.txt");

			while (true)
			{
				std::cout << "Input username : ";
				bool temp = true;
				std::getline(std::cin, username);
				for (int i = 0; i < username.size(); i++)
				{
					if (username[i] == ':')
					{
						std::cout << "Username cannot contain ':' character." << std::endl;
						temp = false;
						break;
					}
					else if (username[i] == ' ')
					{
						std::cout << "Username cannot contain space character." << std::endl;
						temp = false;
						break;
					}
				}
				if (temp == true)
				{
					break;
				}
			}
			while (true)
			{
				std::cout << "Input password : ";
				bool temp = true;
				std::getline(std::cin, password);
				for (int i = 0; i < password.size(); i++)
				{
					if (password[i] == ' ')
					{
						std::cout << "Password cannot contain space character." << std::endl;
						temp = false;
						break;
					}
				}
				if (temp == true)
				{
					break;
				}
			}

			std::ofstream writeFile("Credentials.txt");
			password = sha512(password);
			writeFile << username << ":" << password;
			writeFile.close();
		}
	}

#ifdef __linux__
	sock;
	int client;
	struct sockaddr_in sockInfo;
	int opt = 1;
	int sockInfoLen = sizeof(sockInfo);

	sock= socket(AF_INET,SOCK_STREAM,0);
	if (sock == 0)
	{
		std::cout<<"socket failed !"<<std::endl;
		return 1;
	}

	int setsockoptResult= setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if (setsockoptResult)
	{
		std::cout<<"setsockopt failed !"<<std::endl;;
		return 1;
	}
	sockInfo.sin_family = AF_INET;
	sockInfo.sin_addr.s_addr = INADDR_ANY;
	sockInfo.sin_port = htons(port);

	int bindResult=bind(sock, (struct sockaddr *)&sockInfo,sizeof(sockInfo));
	if (bindResult<0)
	{
		std::cout<<"bind failed !"<<std::endl;
		return 1;
	}

	int listenResult= listen(sock,3);
	if (listenResult< 0)
	{
		std::cout<<"listen failed !"<<std::endl;
		return 1;
	}
#else
	#define WIN32_LEAN_AND_MEAN
	#pragma comment (lib, "Ws2_32.lib")

	WSADATA wsaData;
	int iResult;

	SOCKET sock = INVALID_SOCKET;
	SOCKET client = INVALID_SOCKET;

	struct addrinfo* result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(sock, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(sock, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 1;
	}
#endif
	bool running=true;
	std::thread exitclient(ExitClient,&running);
	while(running){
#ifdef __linux__
		client = accept(sock, (struct sockaddr *)&sockInfo,(socklen_t*)&sockInfoLen);
		if (client<0)
		{
			std::cout<<"accept failed !"<<std::endl;
			return 1;
		}
#else
		client = accept(sock, NULL, NULL);
		if (client == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			return 1;
		}
#endif
		signal(SIGINT, Server_Ctrl_Handler);
		std::string userEntered = SocketRead(client);
		SocketSend(client, "T");
		std::string passEntered = SocketRead(client);
		if (username==userEntered && passEntered==password) {
			//OK
			SocketSend(client, "Y");
			std::string name = SocketRead(client);
			bool NameOk = true;
			for (int i = 0; i < clients.size(); i++) {
				if (clients[i]->GetName() == name) {
					NameOk = false;
					break;
				}
			}
			if (NameOk) {
				SocketSend(client, "Y");
				Client* newClient = new Client(client, name);
				clients.push_back(newClient);
				newClient->StartThread();
				std::cout << "Client " << name << " accepted!" << std::endl;
			}
			else {
				SocketSend(client, "N");
			}
		}
		else {
			SocketSend(client, "N");
		}
	}
	exitclient.join();
	return 0;	
}

void ExitClient(bool* running){
	while((*running)){
		std::vector<int> pending;
		for(int i=0; i< clients.size();i++){
			if(clients[i]->exit){
				clients[i]->JoinThread();
				pending.push_back(i);
				std::cout<<"Client "<<clients[i]->GetName()<<" exited"<<std::endl;
			}
		}
		for(int i=pending.size()-1;i>=0;i--){
			clients.erase(clients.begin()+pending[i]);
		}
	}
}

void Server_Ctrl_Handler(int sig) {
	if (sig == 2) {
		//Ctrl C
		running = false;
		for (int i = 0; i<clients.size();i++) {
			SocketSend(clients[i]->GetId(), "exit");
		}
		std::cout << "Server closed" << std::endl;
		exit(0);
	}
}

std::pair<std::string, std::string> Split(std::string content, char delimiter)
{
	std::string first;
	std::string second;
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i] != delimiter)
		{
			first += content[i];
		}
		else
		{
			for (int j = i + 1; j < content.size(); j++)
			{
				second += content[j];
			}
			break;
		}
	}
	return std::make_pair(first, second);
}