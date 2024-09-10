#include "../inc/client.hpp"

void Server::Kick(Client &client){
	std::cout << client.get_fd() << "entrei KICK\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	it++;
	if (it != tmp.end()){
		if (is_channel((*it)) == true && channel_already_exists((*it)) == true){
			//channel existes;
			channel *tmp_channel = return_channel((*it));
			it++;
			if (it == tmp.end()){
				send_to_server(ERR_NEEDMOREPARAMS, client);
				return;
			}
			if (is_client_op(client, *tmp_channel) == true){
				Client *tmp_client = return_client((*it));
				if (is_client_on_channel(*tmp_client, *tmp_channel) == true){
					send_to_all_channel(":" + client.get_nickname() + " KICK " + tmp_channel->get_name() + " " + tmp_client->get_nickname() + " :" + reasons(tmp, 3), *tmp_channel);
					tmp_channel->remove_client(tmp_client);
				}
				else
					send_to_server("441 :" + tmp_client->get_nickname() + " " + tmp_channel->get_name() + " :is not on the channel!" , client);
			}
			else{
				send_to_server("482 :" + tmp_channel->get_name() + " :You're not a channel operator" , client);
			}
		}
		else{
			send_to_server("403 :" + (*it) + ":That channel does not exist!", client);
		}
	}

}