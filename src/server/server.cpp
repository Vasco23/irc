#include "../../inc/server.hpp"
#include "../../inc/client.hpp"
#include "../../inc/channel.hpp"

Server::Server(std::string _ip, std::string _pass) : ip(_ip), pass(_pass){
	poll_num = 0;
}

Server::~Server(){
}

int Server::create_server(){
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		close (socket_fd);
		return 1;
	}
	if (socket_fd == -1)
		return 1;
	hint.sin_addr.s_addr = inet_addr("127.0.0.1");
	hint.sin_family = AF_INET;
	hint.sin_port = htons(atoi(ip.c_str()));
	inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);
	if (bind(socket_fd, (const sockaddr*)&hint, sizeof(hint)) == -1){
		std::cout << "error in binding\n";
		return 1;
	}
	if (listen(socket_fd, SOMAXCONN) == -1)
		return 1;
	server_loop();
	close(socket_fd);
	return 0;
}

int Server::client_joined(){
	sockaddr_in new_client;
	socklen_t new_client_len = sizeof(new_client);
	//socklen_t	new_clientSize
	int new_client_fd = accept(socket_fd, (sockaddr*)&new_client, &new_client_len);
	if (new_client_fd == -1)
		return 1;
	Client *tmp = new Client(new_client, new_client_len, new_client_fd);
	clients.push_back(tmp);
	struct pollfd client_pollfd;
	client_pollfd.fd = new_client_fd;
	tmp->set_ip(inet_ntoa(new_client.sin_addr));
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	poll_fds[poll_num] = client_pollfd;
	poll_num++;
	return 0;
}

void Server::update_poll_fds(int i){
	for (; i < poll_num - 1; i++){
		poll_fds[i] = poll_fds[i + 1];
	}
	poll_num--;
}

void Server::read_from_client(int i, std::string buffer){
	int it;
	clients.at(i - 1)->set_input(buffer);
	if ((it = buffer.find("\r\n")) != (int)std::string::npos){
		parser_comand(i - 1, clients.at(i - 1)->get_input());
		if ((int)clients.size() <= i - 1){
			return;
		}
		clients.at(i - 1)->set_input_clear();
		clients.at(i - 1)->set_parsed_input_clear();
	}
}

void Server::server_loop(){
	server_pollfd.fd = socket_fd;
    server_pollfd.events = POLLIN; 
    poll_fds[poll_num] = server_pollfd;
	poll_num++;
	while (true){
		int poll_count = poll(poll_fds, poll_num , -1);
		if (poll_count < 0) {
            ////close func here ->
            exit(EXIT_FAILURE);
        }
		for (int i = 0; i < poll_num; i++) {
            if (poll_fds[i].revents && (POLLIN)) {
                if (poll_fds[i].fd == socket_fd) {
					std::cout << "///////// ENTREI AQUI FDS\n";
                    if (client_joined() == 0){
						std::cout << "111111111111111111111\n";
					}
                } 
				else {
					char buffer[1024] = {0};
					int valread = recv(poll_fds[i].fd, buffer, 1024, 0);
					if (valread > 0)
						read_from_client(i, buffer);
                }
            }
        }
    }
}

void Server::parser_comand(int i, std::string str){
	clients.at(i)->set_parsed_input(str);
	clients.at(i)->print_parsed_input();
	find_command(i);
}

void Server::find_command(int i){
	int j = 0;
	Client *tmp = clients.at(i);
	std::string commads[11] = {"PASS", "NICK", "USER", "JOIN", "PART", "PRIVMSG", "TOPIC", "KICK", "INVITE", "MODE", "QUIT"};
	f func[11] = {&Server::Pass, &Server::Nick, &Server::User, &Server::Join, &Server::Part, &Server::Privmsg, &Server::Topic, &Server::Kick, &Server::Invite, &Server::Mode, &Server::Quit};
	std::vector<std::vector<std::string> > copy = clients.at(i)->get_parsed_input();
	std::vector<std::vector<std::string> >::iterator it = copy.begin();
	for (; it != copy.end(); it++){
		while (j < 11){
			if ((*it).front().compare(commads[j]) == 0)
				(this->*(func[j]))(*tmp);
			j++;
		}
		tmp->update_parsed_input();
		j = 0;
	}
}

/*
	
	

*/



/////send stuff back////
void Server::send_to_server(std::string str, Client &client){
	str += "\r\n";
	send(client.get_fd(), str.c_str(), str.length(), 0);
}

void Server::send_to_channel(std::string str, Client &client, channel &channel){
	std::vector<std::pair<Client *, bool> >tmp = channel.get_clients();
	std::vector<std::pair<Client *, bool> >::iterator it = tmp.begin();
	for (; it != tmp.end(); it++){
		if (client.get_nickname().compare((*it).first->get_nickname()) == 0)
			continue;
		else{
			send_to_server(str, *(*it).first);
		}
	}
}

void Server::send_to_all_channel(std::string str, channel &channel){
	std::vector<std::pair<Client *, bool> >tmp = channel.get_clients();
	std::vector<std::pair<Client *, bool> >::iterator it = tmp.begin();
	for (; it != tmp.end(); it++){
		std::cout << (*it).first->get_fd() << std::endl;
		send_to_server(str, *(*it).first);
		//send((*it).first->get_fd(), str.c_str(), str.length(), 0);
	}
}

void Server::send_to_all(std::string str){
	std::vector<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		std::cout << (*it)->get_fd() << std::endl;
		send_to_server(str, *(*it));
		//send((*it).first->get_fd(), str.c_str(), str.length(), 0);
	}
}


bool Server::is_client_on_server(std::string str){
	std::vector<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		if ((*it)->get_nickname().compare(str) == 0){
			return true;
		}
	}
	return false;
}