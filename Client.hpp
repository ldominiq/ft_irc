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

private:
	Client();
    int                                 _clientFd;
    std::string                         _nickname;
    std::string                         _username;
    std::map<std::string, Channel *>    _channels;
};


#endif //FT_IRC_CLIENT_HPP
