//
// Created by Lucas on 24-Feb-23.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP

#include <string>
#include <list>
#include <map>
#include <utility>
#include <iostream>
#include <sstream>

#include "Channel.hpp"
#include "TcpListener.hpp"

class TcpListener;
class Channel;

class Client {
public:
    Client(int fd, std::string hostname);
    ~Client();

	bool								set_nickname(const std::string &nick, TcpListener	&SERV);
	bool								set_userdata(const std::string &userdata);
	void								set_registered();
	void								set_connected();
	void								set_operator();
	int									get_fd() const { return _clientFd;};
	bool								is_registered() const { return _registered;};
	bool								is_connected() const { return _connected;};
	bool 								is_operator() const { return _operator; }
	std::string							get_nick() { return _nickname.empty() ? "" : _nickname; }
	std::string							get_username() { return _username.empty() ? "" : _username; }
	std::string 						get_hostname() { return _hostname.empty() ? "" : _hostname; }
	void								get_infos();
	std::map<std::string, Channel *>& 	get_channels() { return _channels; }
	void 								join_channel(Channel *channel);
	void 								leave_channel(Channel &channel);
	bool								in_channel(const std::string& channel_name);
private:
	Client() {};
	bool 								_registered;
	bool								_connected;
	bool 								_operator;
    int                                 _clientFd;
    std::string                         _nickname;
    std::string                         _username;
    std::map<std::string, Channel *>    _channels;
	std::string 						_hostname;
};


#endif //FT_IRC_CLIENT_HPP
