#include "../inc/server.hpp"

bool running = true;

static bool isNumber(std::string str){
	for (int i = 0; i < (int)str.length(); i++){
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

static bool checkInput(std::string input){
	if(isNumber(input) && (atoi(input.c_str()) >= 1024 && atoi(input.c_str()) <= 65535))
		return true;
	return false;
}

static bool checkPass(std::string pass){
	if (pass.empty())
		return true;
	return false;
}

int main(int ac, char **av){
	if (ac != 3)
		return (1);
	if (!checkInput(av[1]) || checkPass(av[2]))
		return (1);
	Server *server = new Server(av[1], av[2]);
	server->create_server();
	delete server;
}