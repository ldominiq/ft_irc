//
// Created by Lucas on 24-Feb-23.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

#include <string>
#include <list>
#include <map>
#include "Channel.hpp"

class Client {
public:
    Client(int fd);
    ~Client();

	bool	set_nickname(const std::string &nick, std::list<Client *>     &_clients);
	std::string get_nick() {
    	return _nickname.empty() ? "" : _nickname;
	}
	int get_fd() const { return _clientFd;};
	bool get_status() const { return _registered;};
private:
	Client();
	bool 								_registered;
    int                                 _clientFd;
    std::string                         _nickname;
    std::string                         _username;
    std::map<std::string, Channel *>    _channels;
};


#endif //FT_IRC_CLIENT_HPP
