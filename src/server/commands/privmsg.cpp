#include "../inc/client.hpp"

Client* Server::return_client(std::string str){
	std::vector<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		if ((*it)->get_nickname() == str)
			return (*it);
	}
	return NULL;
}

/*
    
*/

void Server::Privmsg(Client &client){
	std::cout << client.get_fd() << "entrei PRIVMSG\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	Bot bot;
	it++;
	if(it != tmp.end()){
		if (is_channel((*it)) == true){
			channel *tmp_channel = return_channel((*it));
			if (!tmp_channel)
				return;
			send_to_channel(":" + client.get_nickname() + " PRIVMSG " + (*it) + " " + reasons(tmp, 2), client, *tmp_channel);
			bot.verifyMessage(reasons(tmp, 2), client, *tmp_channel, *this);
			}
		else{
			Client *tmp_client = return_client((*it));
			if (!tmp_client)
				return;
			send_to_server(":" + client.get_nickname() + " PRIVMSG " + (*it) + " " + reasons(tmp, 2), *tmp_client);
		}
	}
}