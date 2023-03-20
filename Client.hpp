//
// Created by Lucas on 24-Feb-23.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP


#include <string>
#include <map>
#include "Channel.hpp"

class Client {
public:
    Client(int fd);
    ~Client();

	bool	set_nickname(const std::string &nick, std::map<int, Client *>     &_clients);
	std::string get_nick() {
		if (!_nickname.empty())
			return _nickname;
	};
private:
	Client();
    int                                 _clientFd;
    std::string                         _nickname;
    std::string                         _username;
    std::map<std::string, Channel *>    _channels;
};


#endif //FT_IRC_CLIENT_HPP
