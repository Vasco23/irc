#include "../inc/client.hpp"

void Server::Topic(Client &client){
	// std::cout << client.get_fd() << "entrei TOPIC\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	if(tmp.size() < 2){
		send_to_server(ERR_NEEDMOREPARAMS, client); return;
	}
	it++;
	if (is_channel((*it)) && channel_already_exists((*it))){
		channel *tmp_channel = return_channel((*it)); 
		it++;
		if (!is_client_on_channel(client, *tmp_channel)){
			send_to_server("442 :" + client.get_nickname() + " " + tmp_channel->get_name() + ":You're not on that channel", client); return;
		}
		if (tmp_channel->get_topic_restric() == true && is_client_op(client, *tmp_channel) == false){
			send_to_server("482 :" + tmp_channel->get_name() + " :You're not a channel operator!" , client);
			return;
		}
		if (it == tmp.end()){
			if (tmp_channel->get_topic().empty())
				send_to_server("331 :" + tmp_channel->get_name() + " :No topic is set", client);
			else
				send_to_server("332 :" + tmp_channel->get_name() + " :" + tmp_channel->get_topic(), client);
		}
		else{
			tmp_channel->set_topic(reasons(tmp, 2));
			send_to_all_channel(":" + client.get_nickname() + " TOPIC " + tmp_channel->get_name() + " :" + tmp_channel->get_topic(), *tmp_channel);
		}
	}
	else{
		send_to_server("403 :" + (*it) + ":That channel does not exist!", client);
	}
}

