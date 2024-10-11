#include "../inc/client.hpp"

void print_tmp(std::vector<std::string> tmp){
	std::vector<std::string>::iterator it = tmp.begin();
	for(; it != tmp.end(); it++){
		std::cout << "PRINT TMP --->> " << (*it) << std::endl;
	}
}

void Server::Mode(Client &client){
	std::cout << client.get_fd() << " entrei MODE\n";
	std::vector<std::string> tmp = client.get_parsed_input().front();
	//print_tmp(tmp);
	int vector_size = tmp.size();
	/* std::cout << "tmp.size = " << vector_size << std::endl; */
	std::vector<std::string>::iterator it = tmp.begin();
	it++;
	if (it == tmp.end()){
		send_to_server(ERR_NEEDMOREPARAMS, client);
		return;
	}
	channel *tmp_channel = return_channel((*it));
	tmp.erase(tmp.begin());
	vector_size--;
	tmp.erase(tmp.begin());
	vector_size--;
	//std::cout << "tmp.size = " << vector_size << std::endl;
	////wtf size e todo burro
	if (vector_size == 0){
		return;
	}
	Mode_parser(tmp, *tmp_channel, client);
	///find channel
}

void Server::Mode_parser(std::vector<std::string> flags, channel &channel, Client &client){
	std::vector<std::string>::iterator it = flags.begin();
	std::string::iterator it_string;
	std::string arg = "";
	bool set_remove;

	// std::cout << "meu teste -> " << *it << std::endl;
	// it++;
	// std::cout << "meu teste -> " << *it << std::endl;

	// for (; it != flags.end(); it++){
	// 	std::cout << "it: " <<(*it) << std::endl;
	// 	modes.append((*it));
	// 	it_string = (*it).begin();
	// 	if ((*it_string) == '+')
	// 			set_remove = true;
	// 	else if((*it_string) == '-')
	// 		set_remove = false;
	// 	else{
	// 		send_to_server(ERR_NEEDMOREPARAMS, client);
	// 		return;
	// 	}
	// 	it_string++;
	// 	if (it_string == (*it).end()){
	// 		send_to_server(ERR_NEEDMOREPARAMS, client);
	// 		return;
	// 	}
	// 	for (; it_string != (*it).end(); it_string++){
	// 		if ((*it_string) == 'i' || (*it_string) == 't'){
	// 			Mode_exec(set_remove, (*it_string), "", channel, client);
	// 		}
	// 		else if (((*it_string) == 'o' || (*it_string) == 'l' || (*it_string) == 'k')){
	// 			it++;
	// 			if (it == flags.end()){
	// 				send_to_server(ERR_NEEDMOREPARAMS, client);
	// 				return;
	// 			}
	// 			else{
	// 				modes.append((*it));
	// 				Mode_exec(set_remove, (*it_string), (*it), channel, client);
	// 			}
	// 		}
	// 	}

	// }
	if (is_client_op(client, channel) == false){
		send_to_server("482 " + channel.get_name() + " :You're not a channel operator!", client);
		return;
	}
	if ((*it).size() != 2 || ((*it)[0] != '+' && (*it)[0] != '-') || ((*it)[1] != 'i' && (*it)[1] != 't' && (*it)[1] != 'o' && (*it)[1] != 'l' && (*it)[1] != 'k')){
		send_to_server(ERR_NEEDMOREPARAMS, client);
		return;
	}
	char flag = (*it)[1];
	std::string modes = *it; 
	if ((*it)[0] == '+')
		set_remove = true;
	else
		set_remove = false;
	if (++it != flags.end()){
		arg = *it;
	}	
	Mode_exec(set_remove, flag, arg, channel, client);
	//std::cout << "MODES: " <<modes << std::endl;
	
	send_to_all_channel(":" + client.get_nickname() + " MODE " + channel.get_name() + " " + modes + " " + arg, channel);
}

int get_num(std::string str){
	std::string::iterator it = str.begin();
	for (; it != str.end(); it++){
		if (*(it) < '0' || *(it) > '9')
			return -1;
	}
	return std::atoi(str.c_str());

}

void Server::Mode_exec(bool set_remove, char flag, std::string parameters, channel &channel, Client &client){
	if (flag == 'i'){
		channel.set_invite_only(set_remove);
	}
	if (flag == 't'){
		channel.set_topic_restric(set_remove);
	}
	if (flag == 'k'){
		if (parameters.empty()){
			send_to_server(ERR_NEEDMOREPARAMS, client);
			return;
		}
		if (set_remove == false){
			channel.set_pass("");
		}
		else
			channel.set_pass(parameters);
	}
	if (flag == 'o'){
		Client *tmp_client = return_client(parameters);
		if (tmp_client == NULL){
			send_to_server("401 :" + parameters + " :That nick does not exist!" , client);
			return;
		} 
		else if (is_client_on_channel(*tmp_client, channel) == false){
			send_to_server("441 :" + tmp_client->get_nickname() + " " + channel.get_name() + " :is not on the channel!" , client);
			return;
		}
		channel.set_remove_op(tmp_client->get_fd(), set_remove);
	}
	if (flag == 'l'){
		std::string tmp = "l";
		if (set_remove == true){
			int limit = get_num(parameters);
			if (limit < 1 || limit > 9999){
				std::string msg = "472 " + static_cast<std::string>(&flag);
				send_to_server(msg + " :Thats a stupid parameter right there!", client);
			}
			else
				channel.set_user_limit(limit);
		}
		else
			channel.set_user_limit(0);
	}
}
