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

bool Client::set_nickname(const std::string &nick, std::list<Client *>     &clients)
{
	this->_nickname = nick;
	std::cout << "Nickname set to " << this->_nickname << std::endl; // REMOVE WHEN DONE DEBUGGING
	return (true);
}
