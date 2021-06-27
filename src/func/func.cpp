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

void print(std::string s)
{
#ifdef __linux__
	std::cout << s << std::endl;
#else
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == '\033' && i < s.size() - 2)
		{
			if (s[i + 1] == '[')
			{
				std::string temp;
				for (int j = i + 2; j < s.size(); j++)
				{
					if (s[j] != 'm')
					{
						temp += s[j];
					}
					else
					{
						break;
					}
				}
				std::vector<std::string> elements = SplitV(temp, ';');
				HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				for (int j = 0; j < elements.size(); j++)
				{
					//std::cout << elements[j] << std::endl;
					size_t size = elements[j].size();
					//std::cout << "Size : " << size << std::endl;
					int foreground = -1, background = -1;
					int value = std::stoi(elements[j].c_str(), &size);
					std::cout << "STOI : " << value << std::endl;
					switch (std::stoi(elements[j].c_str(), &size))
					{
					case fRed:
					{
						foreground = wRed;
						std::cout << "Setting foreground to red" << std::endl;
						break;
					}
					case bRed:
					{
						background = wRed;
						break;
					}
					case fBlue:
					{
						foreground = wBlue;
						break;
					}
					case bBlue:
					{
						background = wBlue;
						break;
					}
					case fGreen:
					{
						foreground = wGreen;
						break;
					}
					case bGreen:
					{
						background = wGreen;
						break;
					}
					case fYellow:
					{
						foreground = wYellow;
						break;
					}
					case bYellow:
					{
						background = wYellow;
						break;
					}
					case fCyan:
					{
						foreground = wCyan;
						break;
					}
					case bCyan:
					{
						background = wCyan;
						break;
					}
					case fMagenta:
					{
						foreground = wMagenta;
						break;
					}
					case bMagenta:
					{
						background = wMagenta;
						break;
					}
					case fWhite:
					{
						foreground = wWhite;
						break;
					}
					case bWhite:
					{
						background = wWhite;
						break;
					}
					default:
						break;
					}
					
					/*if (std::stoi(elements[j].c_str(), &size) == reset)
					{
						SetConsoleTextAttribute(consoleHandle, 15);
					}
					else
					{
						SetConsoleTextAttribute(consoleHandle, foreground + background * 16);
					}*/
					SetConsoleTextAttribute(consoleHandle, 4);
					std::cout << "Foreground : " << foreground << std::endl;
					std::cout << "Background : " << background << std::endl;
					std::cout << elements[j] << std::endl;
				}
			}
		}
	}
#endif
}
