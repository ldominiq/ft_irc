//
// Created by Lucas on 24-Feb-23.
//
#include "Client.hpp"
#include "MessageHandler.hpp"
#include "utils.hpp"

Client::Client(int fd, std::string hostname) : _registered(false), _connected(false), _clientFd(fd), _nickname(), _username(), _channels(), _hostname(hostname)
{}

Client::~Client()
{}

bool Client::set_nickname(const std::string &nick, TcpListener &SERV) {
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
		std::string nick_msg = "";
		if (this->_nickname.length() > 0) {
			nick_msg = ":" + user_id(_nickname, _username) + " NICK :" + trimmed_nick + "\r\n"; }
        this->_nickname = trimmed_nick;
		if (nick_msg.length() > 0) {
			std::cout << "nick_msg: " << nick_msg << std::endl;
			MessageHandler::HandleMessage(_clientFd, nick_msg); }
        return true;
    } else {
        if (_registered) {
            MessageHandler::numericReply(_clientFd, "433", _username + " " + trimmed_nick + " :Nickname is already in use");
        } else {
			MessageHandler::HandleMessage(_clientFd, trimmed_nick + " " + trimmed_nick + " :Nickname is already in use");
		}
		SERV._disconnect_client(this->_clientFd);
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

bool Client::set_userdata(const std::string &userdata)
{
	if (userdata.length() <= 5) { // A.K.A if there is nothing after the command
		MessageHandler::numericReply(_clientFd, "461",
									 _username + " " + "USER" + " :Not enough parameters");
		return (false); }

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

void Client::join_channel(Channel *channel) {
	channel->add_user(this);
	this->_channels.insert(std::pair<std::string, Channel *>(channel->get_name(), channel));
}

bool Client::in_channel(const std::string& channel_name) {
	std::map<std::string, Channel *> & channels = this->get_channels();

	std::map<std::string, Channel *>::iterator it = channels.find(channel_name);
	if (it == channels.end())
		return false;
	return true;
}

void Client::leave_channel(const std::string &channel_name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
		return;
	Channel *channel = it->second;
	channel->remove_user(this->get_fd());
	_channels.erase(it);

}
