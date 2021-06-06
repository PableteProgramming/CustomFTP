#include <func.hpp>

class Client{
private:
	std::string name;
	std::thread* reading;
#ifdef __linux__
	int id;
public:
	Client(int,std::string);
	int GetId();
#else
	SOCKET id;
public:
	Client(SOCKET, std::string);
	SOCKET GetId();
#endif
	bool exit;
	void StartThread();
	std::string GetName();
	void JoinThread();
};

void Read(Client*);
void exec(std::string);
