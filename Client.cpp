//
// Created by Lucas on 24-Feb-23.
//

#include "Client.hpp"
#include <utility>
#include <iostream>

Client::Client(int fd) : _registered(false), _clientFd(fd), _nickname(), _username(), _channels()
{}

Client::~Client()
{}

bool Client::set_nickname(const std::string &nick, std::list<Client *>     &clients)
{
	std::string trimmed_nick = nick.substr(5);
	if (trimmed_nick.length() < 6)
	{ // todo: check that message is valid per rfc
		numericReply(_clientFd, 431, "No nickname given");
		return (false);
	}
	if (is_valid_nick(trimmed_nick) == false)
	{
		numericReply(_clientFd, 432, trimmed_nick + " Erroneous nickname");
		return (false);
	}
	if (!TcpListener::_nickname_available(const_cast<std::string &>(trimmed_nick)))
	{// todo: add <client> real name/hostname to the error message, depending on if registered or not
		if (_registered)
			numericReply(_clientFd, 433, _username + " " + trimmed_nick + " Nickname is already in use");
		else
			numericReply(_clientFd, 433, trimmed_nick + " " + trimmed_nick + " Nickname is already in use");
		return (false);
	}
	// todo: TRIM THE NICKNAME & check that the way you store the string is valid
	this->_nickname = trimmed_nick;
	std::cout << "Nickname set to " << this->_nickname << std::endl; // REMOVE WHEN DONE DEBUGGING
	return (true);
}


