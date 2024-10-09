#include "../../inc/server.hpp"
#include "../../inc/client.hpp"
#include "../../inc/channel.hpp"

bool server_on = true;

Server::Server(std::string _ip, std::string _pass) : ip(_ip), pass(_pass){

}

Server::~Server(){
	// std::cout << "server destructer called!" << std::endl;
	//delete this;
}

int Server::create_server(){
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	if (socket_fd == -1)
		return 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
		close (socket_fd);
		return 1;
	}
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
	registerSignal();
	server_loop();
	clean_server();
	
	// close(socket_fd);
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
	poll_fds.push_back(client_pollfd);
	// poll_fds[poll_num] = client_pollfd;
	// poll_num++;
	return 0;
}

void Server::update_poll_fds(int fd){
	for (size_t i = 0; i < poll_fds.size(); i++) {
		if (poll_fds[i].fd == fd) {
			poll_fds.erase(poll_fds.begin() + i);
			break;
		}
	}
}

void Server::read_from_client(int i, std::string buffer){
	int it;
	clients.at(i - 1)->set_input(buffer);
	if ((it = buffer.find("\r\n")) != (int)std::string::npos || (it = buffer.find("\n")) != (int)std::string::npos){
		parser_comand(i - 1, clients.at(i - 1)->get_input());
		if ((int)clients.size() <= i - 1){
			return;
		}
		clients.at(i - 1)->set_input_clear();
		clients.at(i - 1)->set_parsed_input_clear();
	}

}

void Server::signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Recebido SIGINT (Ctrl-C). Fechando o servidor com segurança..." << std::endl;
		server_on = false;
    }
}

void Server::ignoreSignal(int signal) {
    if (signal == SIGQUIT) {
        std::cout << "Sinal SIGQUIT ignorado (Ctrl-D)." << std::endl;
    }
}


void Server::registerSignal(){
	signal(SIGINT, Server::signal_handler);
	signal(SIGQUIT, Server::ignoreSignal);
}

void Server::server_loop(){
	server_pollfd.fd = socket_fd;
    server_pollfd.events = POLLIN; 
    poll_fds.push_back(server_pollfd);
	while (server_on){
		//for (size_t i = 0; i < poll_fds.size(); i++) {
		//	std::cout << "checking " << poll_fds[i].fd << " as POLL ";
		//	if (poll_fds[i].events & POLLOUT)
		//		std::cout << "OUT" << std::endl;
		//	else
		//		std::cout << "IN" << std::endl;
		//}

		int poll_count = poll(poll_fds.data(), poll_fds.size() , -1);
		if (poll_count < 0) {
            ////close func here ->
           return;
        }
		for (size_t i = 0; i < poll_fds.size(); i++) {
            if (poll_fds[i].revents & POLLIN) {
                if (poll_fds[i].fd == socket_fd) {
                    if (client_joined() == 1){
						continue;
					}
					// else {
					// 	if (clients.size() == 2) {
					// 		send_to_server("005 :", *clients.at(clients.size() - 1));
					// 		Quit(*clients.at(clients.size() - 1));
					// 	}
					// }
                } 
				else {
					char buffer[1025] = {0};
					int valread = recv(poll_fds[i].fd, buffer, 1024, 0);
					// std::cout << "valread: " << valread << std::endl;
					// std::cout << "buffer: " << std::string(buffer) << std::endl << std::endl << std::endl;
					if (valread > 0)
						read_from_client(i, buffer);
					else if (valread == 0)
						Quit(*clients.at(i - 1));
                }
            }
			else if (poll_fds[i].revents & POLLOUT){
				if (clients.at(i - 1)->flush())
					poll_fds[i].events = POLLIN;
				//std::cout << "setting " << poll_fds[i].fd << " to input" << std::endl;
			}
        }
    }
}

void Server::parser_comand(int i, std::string str){
	if (str.empty() == true || str == "\n")	
		return;
	clients.at(i)->set_parsed_input(str);
	//clients.at(i)->print_parsed_input();
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
		bool exists = false;
		for (size_t i = 0; i < clients.size(); i++) {
			if (clients[i] == tmp) {
				exists = true;
				break;
			}
		}
		if (exists)
			tmp->update_parsed_input();
		j = 0;
	}
}

void Server::clean_server(){
	std::vector<Client *>::iterator it = clients.begin();
	std::vector<channel *>::iterator it2 = channels.begin();
	for (; it != clients.end(); it++){
		delete *it;
	}
	for (; it2 != channels.end(); it2++){
		delete *it2;
	}
}

/////send stuff back////
void Server::send_to_server(std::string str, Client &client){
	std::vector<struct pollfd>::iterator it =  poll_fds.begin();
	str += "\r\n";
	//send(client.get_fd(), str.c_str(), str.length(), 0);
	client.set_output(str);
	//std::cout << "setting " << client.get_fd() << " to output" << std::endl;
	for(; it != poll_fds.end(); ++it){
		if ((*it).fd == client.get_fd()){
			(*it).events = POLLOUT;
			return;
		}
	}
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
		//std::cout << (*it).first->get_fd() << std::endl;
		send_to_server(str, *(*it).first);
		//send((*it).first->get_fd(), str.c_str(), str.length(), 0);
	}
}

void Server::send_to_all(std::string str){
	std::vector<Client *>::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		//std::cout << (*it)->get_fd() << std::endl;
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

int Server::getSockFd(){
	return socket_fd;
}