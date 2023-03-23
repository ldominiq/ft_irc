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

// todo: test thoroughly, make sure all situations are covered
bool Client::set_nickname(const std::string &nick, std::list<Client *>     &clients, TcpListener	&SERV)
{
	std::string trimmed_nick = nick.substr(5);
	if (trimmed_nick.length() < 6) { // A.K.A if there is no nickname after the NICK command
		MessageHandler::numericReply(_clientFd, 431, "No nickname given");
		return (false);
	}
	if (!is_valid_nick(trimmed_nick)) {
		MessageHandler::numericReply(_clientFd, 432, trimmed_nick + " Erroneous nickname");
		return (false);
	}
	if (!SERV._nickname_available(const_cast<std::string &>(trimmed_nick))) {
		if (_registered)
			MessageHandler::numericReply(_clientFd, 433, _username + " " + trimmed_nick + " Nickname is already in use");
		else
			MessageHandler::numericReply(_clientFd, 433, trimmed_nick + " " + trimmed_nick + " Nickname is already in use");
		return (false);
	}
	// todo: TRIM THE NICKNAME & check that the way you store the string is valid
	this->_nickname = trimmed_nick;
//	std::cout << "Nickname set to " << this->_nickname << std::endl; // REMOVE WHEN DONE DEBUGGING
	return (true);
}

static bool is_valid_username(std::string u) {
	// check length
	if (u.length() < 1 || u.length() > 9) {
		return false;
	}
	// check characters
	for (std::size_t i = 0; i < u.length(); i++) {
		char c = u[i];
		if (!std::isalnum(c) && c != '-' && c != '[' && c != ']' && c != '\\' &&
			c != '`' && c != '^' && c != '{' && c != '}' && c != '|') {
			return false;
		}
	}
	return true;
}

bool Client::set_userdata(const std::string &userdata, TcpListener	&SERV)
{
	std::istringstream splatout(userdata);

	if (userdata.length() < 6) { // A.K.A if there is nothing after the command
		MessageHandler::numericReply(_clientFd, 461,
									 _username + " " + "USER" + " :Not enough parameters");
		return (false);
	}
	std::string u;
	if (splatout >> u && !is_valid_username(u) { // todo: check that msg format is correct
		MessageHandler::numericReply(_clientFd, 432, u + " Erroneous nickname");
		return (false); }
	else {
		_username = u; }
}


