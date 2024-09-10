#include "channel.hpp"
#include "client.hpp"

channel::channel(std::string _name) : name(_name){
	password = "";
	topic = "";
	user_limit = 0;
	invite_only = false;
	topic_restric = false;
}

channel::~channel(){
}


///get///
std::string channel::get_name(){
	return this->name;
}

std::vector<std::pair<Client *, bool> > channel::get_clients(){
	return this->clients;
}

std::string channel::get_pass(){
	return this->password;
}

std::string channel::get_topic(){
	return this->topic;
}

int	channel::get_user_limit(){
	return this->user_limit;
}

std::vector<int> channel::get_invited_fds(){
	return this->invited_fds;
}

bool channel::get_invite_only(){
	return this->invite_only;
}

bool channel::get_topic_restric(){
	return this->topic_restric;
}

int	channel::get_num_of_ops(){
	std::vector<std::pair<Client *, bool> >::iterator it = clients.begin();
	int op_num = 0;
	for (; it != clients.end(); it++){
		if ((*it).second == true)
			op_num++;
	}
	return op_num;
}

///set///

void channel::set_name(std::string _name){
	this->name = _name;
}

void channel::set_pass(std::string _pass){
	this->password = _pass;
}

void channel::set_topic(std::string _topic){
	this->topic = _topic;
}

void channel::set_user_limit(int limit){
	this->user_limit = limit;
}

void channel::set_invite_only(bool set){
	this->invite_only = set;
}

void channel::set_topic_restric(bool set){
	this->topic_restric = set;
}

///add///
void channel::add_client(std::pair<Client *, bool> tmp){
	this->clients.push_back(tmp);
}

void channel::add_fd(int fd){
	this->invited_fds.push_back(fd);
}

//////remove//////
void channel::remove_client(Client *client){
	std::vector<std::pair<Client *, bool> >::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		if ((*it).first->get_fd() == client->get_fd()){
			clients.erase(it);
			return;
		}
	}
}

void channel::remove_invited_fd(int fd){
	std::vector<int>::iterator it = invited_fds.begin();
	for (; it != invited_fds.end(); it++){
		if ((*it) == fd){
			invited_fds.erase(it);
			return;
		}
	}
}

//////helpers///////

bool channel::is_client_invited(int fd){
	std::vector<int>::iterator it = invited_fds.begin();
	for (; it != invited_fds.end(); it++){
		if ((*it) == fd){
			return true;
		}
	}
	return false;
}

void channel::set_remove_op(int fd, bool set_remove){
	std::vector<std::pair<Client *, bool> >::iterator it = clients.begin();
	for (; it != clients.end(); it++){
		if ((*it).first->get_fd() == fd){
			if (set_remove == true)
				(*it).second = true;
			else
				(*it).second = false;
			return;
		}
	}
}

std::string channel::get_current_modes() {
    std::string modes = "+";
    if (invite_only) {
        modes += "i";
    }
    if (get_num_of_ops() > 0) {
        modes += "0";
    }
    if (topic_restric) {
        modes += "t";
    }
    if (!password.empty()) {
        modes += "k ";
    }
    if (user_limit > 0) {
        modes += "l ";
    }
    return modes;
}