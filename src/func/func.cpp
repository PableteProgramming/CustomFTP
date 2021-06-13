#include <func.hpp>

#ifdef __linux__
std::string SocketRead(int sock){
	char temp[1024]={0};
	read(sock,temp,sizeof(temp));
	return std::string(temp);
}

void SocketSend(int sock, std::string message){
	send(sock,message.c_str(),strlen(message.c_str()),0);
}
#else
std::string SocketRead(SOCKET sock) {
	char temp[1024] = { 0 };
	recv(sock, temp, sizeof(temp),0);
	return std::string(temp);
}

void SocketSend(SOCKET sock, std::string message) {
	send(sock, message.c_str(), strlen(message.c_str()), 0);
}
#endif

std::vector<std::string> SplitV(std::string s, char c) {
	std::vector<std::string> r;
	r.clear();
	std::string temp = "";

	for (int i = 0; i < s.size(); i++) {
		if (s[i] == c) {
			if (EndStrip(temp) != "") {
				r.push_back(temp);
				temp = "";
			}
		}
		else {
			temp += s[i];
		}
	}
	r.push_back(temp);
	return r;
}

std::string EndStrip(std::string s)
{
	bool charFound = false;
	std::string r;
	int index = -1;
	r.clear();

	for (int i = s.size() - 1; i >= 0; i--)
	{
		if (s[i] != '\n' && s[i] != '\t' && s[i] != ' ' && s[i] != '\t' && s[i] != '\v')
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		return "";
	}

	for (int i = 0; i < index + 1; i++)
	{
		r += s[i];
	}

	return r;
}