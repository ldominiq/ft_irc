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

bool Channel::is_user_in_channel(int fd)
{
	for (std::vector<Client *>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if ((*it)->get_fd() == fd)
			return true;
	}
	return false;
}

void Channel::remove_user(int fd)
{
	for (std::vector<Client*>::iterator it = _users.begin(); it != _users.end(); it++) {
		if ((*it)->get_fd() == fd) {
			_users.erase(it);
			return ;
		}
	}
}

void Channel::set_topic(const std::string& topic) {
	_topic = topic;
}

void Channel::set_topic_user(const std::string &user) {
	_topic_user = user;
}

void Channel::set_topic_time(const std::string &time) {
	_topic_time = time;
}
