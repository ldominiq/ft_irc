//
// Created by Lucas on 24-Feb-23.
//

#include "Client.hpp"
#include <utility>
#include <iostream>
#include <sstream>
#include "MessageHandler.hpp"

Client::Client(int fd, std::string hostname) : _registered(false), _connected(false), _clientFd(fd), _hostname(hostname), _nickname(), _username(), _channels()
{}

Client::~Client()
{}

bool Client::set_nickname(const std::string &nick, std::list<Client *> &clients, TcpListener &SERV) {
    if (nick.length() <= 5) {
        MessageHandler::numericReply(_clientFd, "431", ":No nickname given");
        return false;
    }

    std::size_t end = nick.find_first_of("\r\n", 5);
    if (end == std::string::npos) {
        // Handle the case where the string doesn't contain a newline or carriage return
        std::cout << "Invalid input string" << std::endl;
        return false;
    }

    std::string trimmed_nick = nick.substr(5, end - 5);
	std::cout << "trimmed nick: " << trimmed_nick << std::endl;
    if (!is_valid_nick(trimmed_nick)) {
        std::cout << "Not a valid nickname" << std::endl;
        MessageHandler::numericReply(_clientFd, "432", trimmed_nick + " :Erroneous nickname");
        return false;
    }

    if (SERV._nickname_available(const_cast<std::string &>(trimmed_nick))) {
        this->_nickname = trimmed_nick;
        return true;
    } else {
        if (_registered) {
            MessageHandler::numericReply(_clientFd, "433", _username + " " + trimmed_nick + " :Nickname is already in use");
        } else {
            MessageHandler::numericReply(_clientFd, "433", trimmed_nick + " " + trimmed_nick + " :Nickname is already in use");
        }
        return false;
    }
}

void Client::set_registered() {
	this->_registered = true;
}

void Client::set_connected() {
	this->_connected = true;
}

static bool is_valid_username(std::string u) {
	// check length
	std::cout << "is_valid " << u << std::endl;
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
	if (userdata.length() <= 5) { // A.K.A if there is nothing after the command
		MessageHandler::numericReply(_clientFd, "461",
									 _username + " " + "USER" + " :Not enough parameters");
		return (false); }

	// HELLO THIS IS A TEST
	std::istringstream splitout(userdata);
	std::string u;

	splitout >> u;
	if (splitout >> u && !is_valid_username(u)) { // todo: check that msg format is correct
		_username = _nickname; } // nickname is a fallback value for username when incorrect
	else {
		_username = u;
		std::cout << _username << std::endl;
	}
	splitout >> u; splitout >> u;
	if (!u.empty())
		_hostname = u;
	return (true);
}

void Client::get_infos() {
	std::cout << "CLIENT INFOS" << std::endl;
	std::cout << "fd: " << this->get_fd() << std::endl;
	std::cout << "nick: " << this->get_nick() << std::endl;
	std::cout << "user: " << this->get_username() << std::endl;
	std::cout << "hostname" << this->get_hostname() << std::endl;
	std::cout << "registered: " << this->is_registered() << std::endl;
	std::cout << "connected: " << this->is_connected() << std::endl;
}
