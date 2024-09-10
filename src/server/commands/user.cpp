#include "../inc/client.hpp"

void Server::User(Client &client){
	std::cout << client.get_fd() << " entrei no User\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	if (client.get_valid_pass() == false){
		send_to_server(ERR_NOTREGISTERED, client);
		return;
	}
	if (tmp.front().compare("USER") == 0 && tmp.size() == 5){
		if (check_names(tmp.at(1)) == 0 && tmp.at(1).size() < 9){
			if (client.get_username_seted() == false){
				client.set_username(tmp.at(1));
				client.set_username_seted(true);
				if (client.get_nickname_seted() == true && client.get_client_registerd() == false){
					send_to_server("001 :Welcome to the Internet Relay Network" + client.get_nickname() + "!\n" + client.get_username() + "@" + ip, client);
					client.set_client_registerd(true);
				}
			}
		}
		else
			send_to_server(ERR_INVALIDUSERNAME, client);
		if (check_names(tmp.back()) == 0 && tmp.back().size() < 9){
				client.set_realname(tmp.back());
		}
	}
}