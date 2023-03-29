//
// Created by Lucas on 24-Feb-23.
//

#include "Channel.hpp"

Channel::Channel(std::string name) : _name(name), _topic(), _users() {

}

Channel::~Channel() {

}

bool Channel::send_message(std::string nick, std::string &msg)
{
	std::cout << "send_message got " << msg << " from " << nick;
	// DO ALL SORTS OF SHENANIGANS
//	std::vector<Client *>::iterator it = _users.begin();
//	for (; it != _users.end(); it++)
//	{
//
//	}
	return false;
}
