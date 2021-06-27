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

void print(std::string);

const int fRed = 31;
const int bRed = 41;
const int fGreen = 32;
const int bGreen = 42;
const int fYellow = 33;
const int bYellow = 43;
const int fBlue = 34;
const int bBlue = 44;
const int fMagenta = 35;
const int bMagenta = 45;
const int fCyan = 36;
const int bCyan = 46;
const int fWhite = 37;
const int bWhite = 47;

const int reset = 0;
const int bold = 1;
const int underline = 4;
const int inverse = 7;
const int boldOff = 21;
const int underlineOff = 24;
const int inverseOff = 27;

const int wBlack = 0;
const int wBlue = 1;
const int wGreen = 2;
const int wCyan = 3;
const int wRed = 4;
const int wMagenta = 5;
const int wYellow = 14;
const int wWhite = 15;