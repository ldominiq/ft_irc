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

Channel::~Channel() {

}

bool Channel::send_message(std::string sender, std::vector<std::string> &params)
{
	// reformat message before sending
	std::string message = ":" + sender + " PRIVMSG "; // + _name + " :" + params[1] + "\r\n";

	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
		message += *it + " ";
	std::cout << "msg for chan: " << message << std::endl;
	// send message to all users in channel
	for (std::vector<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if ((*it)->get_nick() != sender)
			MessageHandler::HandleMessage((*it)->get_fd(), message + "\r\n");
	}
	return false;
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
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); ) {
		if ((*it)->get_fd() == fd) {
			std::cout << "removing user " << (*it)->get_nick() << std::endl;
			it = _users.erase(it);
		} else {
			++it;
		}
	}
}
