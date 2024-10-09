#include "../inc/client.hpp"

void Server::Pass(Client &client){
	//std::vector<std::vector<std::string> > tmp = client.get_parsed_input();
	//std::vector<std::vector<std::string> >::iterator it = tmp.begin();
	std::vector<std::string> tmp = client.get_parsed_input().front();
	// std::cout << client.get_fd() << " entrei no Pass\n";
	if (client.get_valid_pass() == true){
		std::cout << "invalid pass\n";
		send_to_server(ERR_ALREADYREGISTERED, client);
	}
	if (tmp.front().compare("PASS") == 0){
		if (tmp.size() == 2 && tmp.back().compare(this->pass) == 0){
			send_to_server(RPL_YOUREOPER, client);
			std::cout << "valid pass\n";
			client.set_valid_pass(true);
		}
		else if (tmp.size() == 2 && tmp.back().compare(this->pass) != 0)
			send_to_server(ERR_PASSWDMISMATCH, client);
		else if (tmp.size() == 1)
			send_to_server(ERR_NEEDMOREPARAMS, client);
	}
}