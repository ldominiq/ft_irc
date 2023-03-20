//
// Created by Lucas on 24-Feb-23.
//

#include "Client.hpp"
#include <utility>
#include <iostream>

Client::Client(int fd) : _clientFd(fd), _nickname(), _username(), _channels()
{}

Client::~Client()
{}

bool Client::set_nickname(const std::string &nick, std::map<int, Client *>     &clients)
{
//	std::map<int, Client *>::iterator it;

	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		std::cout << "is empty?: " << it->second->get_nick().empty() << std::endl;
		if (!it->second->get_nick().empty() && it->second->get_nick() == nick)
			return (false);
	}
	this->_nickname = nick;
	std::cout << "Nickname set to " << this->_nickname << std::endl; // REMOVE WHEN DONE DEBUGGING
	return (true);
}
