#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "server.hpp"

class Client;

class channel
{
	private:
		std::string	name;
		std::string password;
		std::string topic;
		int user_limit;
		bool invite_only;
		bool topic_restric;
		std::vector<int> invited_fds;
		std::vector<std::pair<Client *, bool> > clients;

	public:
		channel(std::string _name);
		~channel();

		///get///
		std::string get_name();
		std::string get_pass();
		std::string get_topic();
		int			get_user_limit();
		bool		get_invite_only();
		bool		get_topic_restric();
		std::vector<std::pair<Client *, bool> > get_clients();
		int			get_num_of_ops();
		std::vector<int> get_invited_fds();
		

		///set///
		void set_name(std::string _name);
		void set_pass(std::string _pass);
		void set_topic(std::string _topic);
		void set_user_limit(int limit);
		void set_invite_only(bool set);
		void set_topic_restric(bool set);

		///add///
		void add_fd(int fd);
		void add_client(std::pair <Client *, bool> tmp);

		//////remove//////
		void remove_client(Client  *Client);
		void remove_invited_fd(int fd);

		/////helpers/////
		bool is_client_invited(int fd);
		void set_remove_op(int fd, bool set_remove);
		std::string get_current_modes();
};



#endif