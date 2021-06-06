#include <main.hpp>

int main(int argc, char** argv){
	std::vector<std::string> args= GetArgs(argc,argv);
	if(args.size()<3){
		std::cout<<"Please input two parameters: [ip] [name]"<<std::endl;
		return 1;
	}
	std::string ip= args[1];
	std::string name= args[2];
	int result= client(ip,PORT,name);
	return 0;
}
