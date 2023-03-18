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

bool Client::set_nickname(std::string nick, std::map<int, Client *>     &clients)
{
	std::map<int, Client *>::iterator	it;

	// FIND SEARCH WITH THE KEY, NOT BY VALUE
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->_nickname == nick)
			return (false);
	}
	this->_nickname = std::move(nick);
	std::cout << "Nickname set to " << this->_nickname << std::endl; // REMOVE WHEN DONE DEBUGGING
	return (true);
}
