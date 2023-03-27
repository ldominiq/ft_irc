//
// Created by Lucas on 24-Feb-23.
//

#include "Client.hpp"
#include <utility>
#include <iostream>
#include <sstream>
#include "MessageHandler.hpp"

Client::Client(int fd, std::string hostname) : _registered(false), _clientFd(fd), _hostname(hostname), _nickname(), _username(), _channels()
{}

Client::~Client()
{}

// todo: test thoroughly, make sure all situations are covered
bool Client::set_nickname(const std::string &nick, std::list<Client *>     &clients, TcpListener	&SERV)
{
	if (nick.length() < 6) { // A.K.A if there is no nickname after the NICK command
		MessageHandler::numericReply(_clientFd, "431", "No nickname given");
		return (false);
	}
	std::string trimmed_nick = nick.substr(5);
	if (!is_valid_nick(trimmed_nick)) {
		MessageHandler::numericReply(_clientFd, "432", trimmed_nick + " Erroneous nickname");
		return (false);
	}
	if (SERV._nickname_available(const_cast<std::string &>(trimmed_nick))) {
		this->_nickname = "lucas";
		return (true);
	} else
	{ // todo: figure best way to implement the 'ask again' loop until a valid nickname is given, with this error and all others
		if (_registered)
			MessageHandler::numericReply(_clientFd, "433",
										 _username + " " + trimmed_nick + " Nickname is already in use");
		else
			MessageHandler::numericReply(_clientFd, "433",
										 trimmed_nick + " " + trimmed_nick + " Nickname is already in use");
		return (false);
	}
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

static bool is_valid_realname(std::string n) {

}

bool Client::set_userdata(const std::string &userdata, TcpListener	&SERV)
{
	if (userdata.length() < 6) { // A.K.A if there is nothing after the command
		MessageHandler::numericReply(_clientFd, "461",
									 _username + " " + "USER" + " :Not enough parameters");
		return (false); }

	// HELLO THIS IS A TEST
	std::istringstream splitout(userdata);
	std::string u;

	if (splitout >> u && !is_valid_username(u)) { // todo: check that msg format is correct
		_username = _nickname; } // nickname is a fallback value for username when incorrect
	else {
		_username = u;
		std::cout << _username << std::endl;
	}
	if ((splitout >> u && u != "0") || (splitout >> u && u != "*")) { // todo: check that msg format is correct
		MessageHandler::numericReply(_clientFd, "461",
									 _username + " " + "USER" + " :Not enough parameters");
		return (false); }
	if (splitout >> u && !is_valid_realname(u))
	{

	}
}

void Client::get_infos() {
	std::cout << "CLIENT INFOS" << std::endl;
	std::cout << "fd: " << this->get_fd() << std::endl;
	std::cout << "nick: " << this->get_nick() << std::endl;
	std::cout << "user: " << this->get_username() << std::endl;
	std::cout << "status: " << this->get_status() << std::endl;
}
