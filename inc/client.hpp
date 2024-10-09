#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "server.hpp"
#include "channel.hpp"
#include "Bot.hpp"

class Client
{
	private:
		sockaddr_in addr;
		socklen_t addr_size;
		std::string name;
		std::string nickname;
		std::string username;
		std::string	realname;
		std::string input;
		std::string output;
		std::vector<std::vector<std::string> > parsed_input;
		std::string ip;
		bool		valid_pass;
		bool		nickname_seted;
		bool		username_seted;
		bool		client_registerd;
		int			fd;
		int			badBehavior;

	public:
		Client(sockaddr_in _addr, socklen_t _addr_size, int _fd);
		Client(const Client &copy);
		Client & operator =(const Client &copy);
		~Client();

		///get///
		int get_fd();
		bool get_client_registerd();
		bool get_valid_pass();
		bool get_nickname_seted();
		bool get_username_seted();
		std::vector<std::vector<std::string> > get_parsed_input();
		std::string get_nickname();
		std::string get_username();
		std::string get_realname();
		std::string get_input();
		std::string get_ip();
		void incrementBadBehavior();
		int getBadBehavior();
		

		///set///
		void set_parsed_input(std::string message);
		void set_parsed_input_clear();
		void set_input_clear();
		void set_client_registerd(bool set);
		void set_valid_pass(bool set);
		void set_nickname_seted(bool set);
		void set_username_seted(bool set);
		void set_nickname(std::string nick);
		void set_username(std::string user);
		void set_realname(std::string real);
		void set_input(std::string input);
		void set_ip(std::string ip);	

		//////prints//////
		void print_parsed_input();

		////////////update_parsed_input//////////////
		void update_parsed_input();

};


#endif