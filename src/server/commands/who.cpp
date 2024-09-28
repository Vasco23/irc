#include "../inc/client.hpp"

void Server::Who(Client &client){
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	it++;
	send_to_server(":WHO " + client.get_nickname() + " " + (*it), client);
}