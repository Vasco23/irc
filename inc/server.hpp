#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <fstream>
#include "defines.hpp"
#include "Bot.hpp"
//#include "channel.hpp"

/* #include "client.hpp" */

class Client;

class channel;

class Server
{
	private:
		std::string ip;
		std::string pass;
		int socket_fd;
		sockaddr_in hint;
		std::vector<Client *> clients;
		std::vector<channel *> channels;

		////// events ////////
		/* std::vector<struct pollfd> poll_fds; */
		struct pollfd poll_fds[400];
		int poll_num;
		struct pollfd server_pollfd;

	public:
		Server(std::string _ip, std::string _pass);
		~Server();
		int create_server();
		void server_loop();
		int client_joined();
		void update_poll_fds(int i);
		void read_from_client(int i, std::string buffer);

		///////parser/////////
		void parser_comand(int i, std::string str);

		/////send stuff back////
		void send_to_server(std::string str, Client &client);
		void send_to_channel(std::string str, Client &client, channel &channel);
		void send_to_all_channel(std::string str, channel &channel);
		void send_to_all(std::string str);

		////////commands////////
		void find_command(int i);
		void Pass(Client &Client);
		void Nick(Client &Client);
		void User(Client &Client);
		void Join(Client &Client);
		void Part(Client &Client);
		void Privmsg(Client &Client);
		void Topic(Client &Client);
		void Kick(Client &Client);
		void Invite(Client &Client);
		void Mode(Client &Client);
		void Quit(Client &Client);
		void Who(Client &Client);

		typedef void(Server::*f)(Client &Client);

		///////helpers///////
		void nick_helper(Client &client, std::string str);
		bool channel_already_exists(std::string str);
		bool is_client_on_channel(Client &client, channel &channel);
		bool is_client_on_server(std::string);
		bool is_channel(std::string str);
		channel * return_channel(std::string str);
		bool is_client_op(Client &client, channel &channel);
		std::string reasons(std::vector<std::string> tmp, int i);
		void delete_channel(channel &channel);
		Client * return_client(std::string str);
		void	join_helper(Client &client, channel &channel);
		void	Mode_parser(std::vector<std::string> flags, channel &channel, Client &client);
		void	Mode_exec(bool set_remove, char flag, std::string parameters, channel &channel, Client &client);
};

int check_names(std::string str);

#endif