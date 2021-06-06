#include "Client.hpp"
#include <sha512.hpp>

void Server_Ctrl_Handler(int);
int server(int);
void ExitClient(bool*);
std::pair<std::string, std::string> Split(std::string, char);
