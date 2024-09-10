#include "../inc/client.hpp"

void Server::Invite(Client &client){
	std::cout << client.get_fd() << "entrei INVITE\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	it++;
	if(it == tmp.end()){
		send_to_server(ERR_NEEDMOREPARAMS, client);
		return;
	}
	if (is_client_on_server((*it)) == true){
		Client *tmp_client = return_client((*it));
		//client that i want to invite existes
		it++;
		if(it == tmp.end()){
			send_to_server(ERR_NEEDMOREPARAMS, client);
			return;
		}
		if (channel_already_exists((*it)) == true){
			channel *tmp_channel = return_channel((*it));
			//channel existes
			if (is_client_on_channel(client, *tmp_channel) == false){
				send_to_server("442 :" + tmp_channel->get_name() + " :You're not on that channel!" , client);
				return;
			}
			if (is_client_op(client, *tmp_channel) == false){
				send_to_server("482 :" + tmp_channel->get_name() + " :You're not a channel operator!" , client);
				return;
			}
			if (is_client_on_channel(*tmp_client, *tmp_channel) == true){
				send_to_server("443 :" + tmp_client->get_nickname() + " " + tmp_channel->get_name() + " :Client that you invited already on the channel!" , client);
				return;
			}
			if (tmp_channel->get_user_limit() == 0 || (tmp_channel->get_user_limit() > (int)tmp_channel->get_clients().size())){
				//no user limite || not over the limite;
				tmp_channel->add_fd(tmp_client->get_fd());
				send_to_server(":" + client.get_nickname() + " INVITE " + tmp_client->get_nickname() + " " + tmp_channel->get_name(), *tmp_client);
			}
			else{
				send_to_server("471 " + tmp_channel->get_name() + " :channel is full!", client);
			}
		}

	}
	else{
		send_to_server("401 :" + (*it) + " :That nick does not exist!" , client);
	}
}