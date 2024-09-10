#include "../inc/server.hpp"

int main(int ac, char **av){
	if (ac != 3)
		return (1);
	Server *server = new Server(av[1], av[2]);
	server->create_server();
}