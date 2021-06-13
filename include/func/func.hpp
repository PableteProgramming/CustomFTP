#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <ls.hpp>

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
std::string SocketRead(int);
void SocketSend(int, std::string);
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
std::string SocketRead(SOCKET);
void SocketSend(SOCKET, std::string);
#endif

std::vector<std::string> SplitV(std::string, char);

typedef std::string (*funcPtr)(std::string);

const std::vector<std::pair<std::string, funcPtr>> commands = {
	std::make_pair("ls",&ls)
};

std::string EndStrip(std::string);