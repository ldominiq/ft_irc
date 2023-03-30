//
// Created by Lucas on 24-Feb-23.
//

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(), _users() {

}

Channel::~Channel() {

}

void Channel::add_user(Client *client) {
	this->_users.push_back(client);
}
