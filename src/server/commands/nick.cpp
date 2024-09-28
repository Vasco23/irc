#include "../inc/client.hpp"

void Server::Nick(Client &client){
	std::cout << client.get_fd() << " entrei no Nick\n";
	/* std::vector<std::vector<std::string> > tmp = client.get_parsed_input();
	std::vector<std::vector<std::string> >::iterator it = tmp.begin(); */
	std::vector<std::string> tmp = client.get_parsed_input().front();
	if (client.get_valid_pass() == false){
		std::cout << "aqui!!!\n";
		send_to_server(ERR_NOTREGISTERED, client);
		return;
	}
	if (tmp.size() == 2 &&  check_copy_nick(tmp.back()) == 1){
		send_to_server(ERR_NICKNAMEINUSE, client);
	}
	else if (tmp.size() == 2){
		if (check_names(tmp.back()) == 1 || tmp.back().size() > 8){
			send_to_server(ERR_ERRONEUSNICKNAME, client);
		}
		else{
			nick_helper(client, tmp.back());
		}
	}
	else if (tmp.size() == 1)
		send_to_server(ERR_NEEDMOREPARAMS, client);
	else if (tmp.size() > 2){
		send_to_server(ERR_ERRONEUSNICKNAME, client);
	}
}


void Server::nick_helper(Client &client, std::string str){
	if (client.get_nickname_seted() == false){
		client.set_nickname(str);
		client.set_nickname_seted(true);
		std::cout << "set valid" << std::endl;
		if (client.get_username_seted() == true){

			send_to_server("001 :Welcome to the Internet Relay Network " + client.get_nickname() + "!" + client.get_username() + "@" + this->ip, client);
			client.set_client_registerd(true);
		}
	}
	else{
		std::string last_nickname = client.get_nickname();
		client.set_nickname(str);
		send_to_all(":" + last_nickname + "!" + client.get_username() + "@" + client.get_ip() + " NICK :" + str);
	}	
}

int Server::check_copy_nick(std::string str){
	std::vector<Client *>::iterator it2 = clients.begin();
	for (; it2 != clients.end(); it2++){
		if (str == (*it2)->get_nickname())
			return 1;
	}
	return 0;
}
 
int Server::check_names(std::string str){
	std::string::iterator it = str.begin();
	for (; it != str.end(); it++){
		if (((*it) >= '0' && (*it) <= '9') || ((*it) >= 'a' && (*it) <= 'z') || ((*it) >= 'A' && (*it) <= 'Z'))
			continue;
		else
			return 1;
	}
	return 0;
}


