#include <func.hpp>
#include <sha512.hpp>
#include <clear.hpp>

int client(std::string,int,std::string,std::string,std::string);
std::vector<std::string> GetArgs(int,char**);

#ifdef __linux__
void Read(int);
#else
void Read(SOCKET);
#endif

void Client_Ctrl_Handler(int);