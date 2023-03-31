//
// Created by Lucas on 24-Feb-23.
//

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(), _users() {

}

Channel::Channel(const Channel& other) : _name(other._name), _topic(other._topic), _users(other._users) {

}

Channel& Channel::operator=(const Channel& other) {
	if (this != &other) {
		_name = other._name;
		_topic = other._topic;
		_users = other._users;
	}
	return *this;
}

Channel::~Channel() {

}

void Channel::add_user(Client *client) {
	this->_users.push_back(client);
}
