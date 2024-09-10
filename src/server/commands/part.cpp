#include "../inc/client.hpp"

std::string Server::reasons(std::vector<std::string> tmp, int i){
	std::vector<std::string>::iterator it = tmp.begin();
	std::string reason;
	while(i-- > 0)
		it++;
	for (; it != tmp.end(); it++){
		reason.append((*it));
		reason.append(" ");
	}
	return reason;
}

void Server::delete_channel(channel &del){
	std::vector<channel *>::iterator it = channels.begin();
	for (; it != channels.end(); ++it){
		if ((*it)->get_name() == del.get_name()){
			delete((*it));
			std::cout << "NOT SEGV3!\n";
			channels.erase(it);
			return;
		}
	}
}

bool Server::is_client_op(Client &client, channel &channel){
	std::vector<std::pair<Client *, bool> >tmp = channel.get_clients();
	std::vector<std::pair<Client *, bool> >::iterator it = tmp.begin();
	for (; it != tmp.end(); it++){
		if (client.get_fd() ==  (*it).first->get_fd()){
			if ((*it).second == true)
				return true;
		}
	}
	return false;
}

void Server::Part(Client &client){
	std::cout << client.get_fd() << " entrei no Part\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	std::vector<std::string>::iterator it = tmp.begin();
	it++;
	if ((is_channel(*it)) == true){
		channel *tmp_channel = return_channel((*it));
		if (tmp_channel == NULL){
			send_to_server("442 :" + client.get_nickname() + " " + (*it) + " :You're not on that channel", client); 
			return;
		}
		std::cout << "NOT SEGV!\n";
		if(is_client_on_channel(client, *tmp_channel) == true){
			std::cout << "NOT SEGV2!\n";
			std::string msg = ":" + client.get_nickname() + " PART " + (*it) + " :" + reasons(tmp, 2);
			send_to_all_channel(msg, *tmp_channel);
			tmp_channel->remove_client(&client);
			if (is_client_op(client, *tmp_channel) == true && tmp_channel->get_num_of_ops() == 1){
				//last op in channel
				if (return_channel((*it))->get_clients().size() == 1){
					//last client delete channel;
					delete_channel(*tmp_channel);
				}
				else{
					//give op to other client on the channel
					tmp_channel->get_clients().front().second = true;
				}
			}
		}
		else{
			//client is not on channel
			send_to_server("442 :" + client.get_nickname() + " " + tmp_channel->get_name() + ":You're not on that channel", client);
		}
	}
	else{
		//its not a channel NAO SABER SE AQUI TEM DE SER OUTRA MENSAGEM MAS YHA
		send_to_server("403 :" + (*it) + ":That channel does not exist!", client);
	}
}