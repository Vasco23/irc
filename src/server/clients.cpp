#include "../../inc/client.hpp"

Client::Client(sockaddr_in _addr, socklen_t _addr_size, int _fd) : addr(_addr), addr_size(_addr_size), fd(_fd){
	name = "";
	nickname = "";
	username = "";
	realname = "";
	input = "";
	valid_pass = false;
	nickname_seted = false;
	username_seted = false;
	client_registerd = false;
	badBehavior = 0;
}

Client::~Client(){
}

Client::Client(const Client &copy){
	this->addr = copy.addr;
	this->addr_size = copy.addr_size;
	this->name = copy.name;
	this->nickname = copy.nickname;
	this->username = copy.username;
	this->realname = copy.realname;
	this->parsed_input = copy.parsed_input;
	this->valid_pass = copy.valid_pass;
	this->nickname_seted = copy.nickname_seted;
	this->username_seted = copy.username_seted;
	this->client_registerd = copy.client_registerd;
	this->fd = copy.fd;
}

/* Client & Client::operator =(const Client &copy){
	if (this != &copy){
		
	}
} */



///////////////////////////get//////////////////////////


int Client::get_fd(){
	return fd;
}

bool Client::get_client_registerd(){
	return client_registerd;
}

std::vector<std::vector<std::string> > Client::get_parsed_input(){
	return parsed_input;
}

std::string Client::get_nickname(){
	return this->nickname;
}

std::string Client::get_username(){
	return this->username;
}

std::string Client::get_realname(){
	return this->realname;
}

bool Client::get_valid_pass(){
	return this->valid_pass;
}

bool Client::get_nickname_seted(){
	return this->nickname_seted;
}

bool Client::get_username_seted(){
	return this->username_seted;
}

std::string Client::get_input(){
	return this->input;
}

std::string Client::get_ip(){
	return this->ip;
}

int Client::getBadBehavior(){
	return this->badBehavior;
}



////////////////////////set///////////////////////////////

void Client::set_parsed_input(std::string message){
	std::string token;
	std::string token2;
	std::vector<std::string> tmp;
    size_t start = 0, end = 0;
	size_t begin = 0, last = 0;
	//int i = 0;
    while ((end = message.find('\n', start)) != std::string::npos) {
        token = message.substr(start, end - start);
		while ((last = token.find(' ', begin)) != std::string::npos){
			token2 = token.substr(begin, last - begin);
			tmp.push_back(token2);
			token2.clear();
			begin = last + 1;
		}
        token2 = token.substr(begin);
		size_t end1 = token2.find_first_of("\r\n", 0);
		token2 = token2.substr(0, end1);
		if (token2.empty() == false){
			tmp.push_back(token2);
		}
        start = end + 1;
		begin = 0; last = 0;
		parsed_input.push_back(tmp);
		token2.clear();
		token.clear();
		tmp.clear();
    }
}

void Client::set_ip(std::string _ip){
	this->ip = _ip;
}

void Client::set_parsed_input_clear(){
	this->parsed_input.clear();
}

void Client::set_input_clear(){
	this->input.clear();
}

void Client::set_client_registerd(bool set){
	this->client_registerd = set;
}

void Client::set_nickname(std::string nick){
	this->nickname = nick;
}
void Client::set_username(std::string user){
	this->username = user;
}

void Client::set_valid_pass(bool set){
	this->valid_pass = set;
}

void Client::set_nickname_seted(bool set){
	this->nickname_seted = set;
}

void Client::set_username_seted(bool set){
	this->username_seted = set;
}

void Client::set_realname(std::string real){
	this->realname = real;
}

void Client::set_input(std::string _input){
	this->input += _input;
}

void Client::incrementBadBehavior(){
	this->badBehavior++;
}

////////////////////////////////////////

void Client::print_parsed_input(){
	std::vector<std::vector<std::string> >::iterator it;
	std::vector<std::string>::iterator it2;

	for (it = parsed_input.begin(); it != parsed_input.end(); it++){
		std::cout << "//////////////////" << std::endl;
		for(it2 = (*it).begin(); it2 != (*it).end(); it2++){
			std::cout << (*it2) << " ";
		}
		std::cout << std::endl;
	}
}

void Client::update_parsed_input(){
	this->parsed_input.erase(parsed_input.begin());
}

