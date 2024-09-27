#include "../inc/server.hpp"

static bool checkInput(std::string input){
	if(atoi(input.c_str()) >= 1024 && atoi(input.c_str()) <= 65535)
		return true;
	return false;
}


int main(int ac, char **av){
	if (ac != 3)
		return (1);
	if (!checkInput(av[1]))
		return (1);
	Server *server = new Server(av[1], av[2]);
	server->create_server();
}