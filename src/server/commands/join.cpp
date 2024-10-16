#include "../inc/client.hpp"

bool Server::is_channel(std::string str){
	std::string::iterator it = str.begin();
	if (str.size() > 8)
		return false;
	if ((*it) != '#')
		return false;
	it++;
	if(it == str.end())
		return false;
	for (; it != str.end(); it++){
		if (((*it) >= '0' && (*it) <= '9') || ((*it) >= 'a' && (*it) <= 'z') || ((*it) >= 'A' && (*it) <= 'Z'))
			continue;
		else
			return false;
	}
	return true;
}

bool Server::channel_already_exists(std::string str){
	std::vector<channel *>::iterator it = channels.begin();
	for (; it != channels.end(); it++){
		if ((*it)->get_name().compare(str) == 0)
			return true;
	}
	return false;
}

channel* Server::return_channel(std::string str){
	std::vector<channel *>::iterator it = channels.begin();
	for (; it != channels.end(); it++){
		if ((*it)->get_name().compare(str) == 0)
			return (*it);
	}
	return NULL;
}

bool Server::is_client_on_channel(Client &client, channel &channel){
	std::vector<std::pair<Client *, bool> >tmp = channel.get_clients();
	std::vector<std::pair<Client *, bool> >::iterator it = tmp.begin();
	for (; it != tmp.end(); it++){
		if (client.get_fd() == (*it).first->get_fd()){
			return true;
		}
	}
	return false;
}



void Server::Join(Client &client){
	// std::cout << client.get_fd() << " entrei no Join\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	if (client.get_client_registerd() == false){
		send_to_server(ERR_NOTREGISTERED, client);
		return;
	}
	it++;
	std::string name = (*it);
	if (is_channel(name) == true){
			if (channel_already_exists(name) == false){
				channel *tmp = new channel(name);
				channels.push_back(tmp);
				std::pair<Client *, bool> tmp2(&client, true);
				tmp->add_client(tmp2);
				send_to_all_channel(":" + client.get_nickname() + " JOIN " + (*it), *tmp);
			}
			else if(is_client_on_channel(client, *return_channel(name)) == false){
				channel *tmp = return_channel(name);
				if (tmp->get_invite_only() == true && tmp->is_client_invited(client.get_fd()) == false){
					send_to_server("473 " + client.get_nickname() + " " + tmp->get_name() + " :Cannot join channel (+i)\r\n", client);
					return;
				}
				if (tmp->get_user_limit() == 0 || (tmp->get_user_limit() > (int)tmp->get_clients().size()))
				{
					join_helper(client, *tmp);
				}
				else if (tmp->get_user_limit() != 0 && (tmp->get_user_limit() > (int)tmp->get_clients().size())){
					if (tmp->is_client_invited(client.get_fd()) == true){
						tmp->remove_invited_fd(client.get_fd());
						join_helper(client, *tmp);
					}
				}
				else
					send_to_server("471 " + tmp->get_name() + " :channel is full!", client);
			}
		}
	else{
		send_to_server(ERR_NOSUCHCHANNEL, client);
	}
	
}

void Server::join_helper(Client &client, channel &channel){
	std::pair<Client *, bool> tmp2(&client, false);
	if (channel.get_pass().empty() == false){
		std::vector<std::vector<std::string> > input = client.get_parsed_input();
		if (input[0].size() == 3 && input[0][2] == channel.get_pass()){
			channel.add_client(tmp2);
			send_to_all_channel(":" + client.get_nickname() + " JOIN " + channel.get_name(), channel);
			return;
		}
		else{
			send_to_server("475 :" + channel.get_name() + " :Cannot join channel (+k)\r\n", client);
			return;
		}
	}
	channel.add_client(tmp2);
	send_to_all_channel(":" + client.get_nickname() + " JOIN " + channel.get_name(), channel);
	if (channel.get_topic().empty() == false){
		send_to_all_channel(": TOPIC " + channel.get_name() + " :" + channel.get_topic(), channel);
	}
}