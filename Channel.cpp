//
// Created by Lucas on 24-Feb-23.
//

#include "Channel.hpp"

Channel::Channel(std::string name, std::string op) : _name(name), _topic(), _users(), _operator(op) {

}

Channel::Channel(const Channel& other) : _name(other._name), _topic(other._topic), _users(other._users), _operator(other._operator) {

}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_topic = other._topic;
		_users = other._users;
		_operator = other._operator;
	}
	return *this;
}

Channel::~Channel() {}

void Channel::send_to_users(std::string sender, std::string message)
{
	for (std::vector<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->get_nick() != sender)
			MessageHandler::HandleMessage((*it)->get_fd(), message + "\r\n");
	}
}

void Channel::add_user(Client *client) {
	this->_users.push_back(client);
}

void Channel::add_operator(std::string nick) {
	this->_operator = nick;
}

bool Channel::is_user_in_channel(int fd)
{
	for (std::vector<Client *>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((*it)->get_fd() == fd) {
			std::cout << "user found" << std::endl;
			return true;
		}
	}
	std::cout << "user NOT found" << std::endl;
	return false;
}

void Channel::remove_user(int fd)
{
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if ((*it)->get_fd() == fd) {
			std::cout << "removing user " << (*it)->get_nick() << std::endl;
			_users.erase(it);
			return ;
		}
	}
}
