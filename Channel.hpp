//
// Created by Lucas on 24-Feb-23.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP


#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"

class Client;

class Channel {
public:
    Channel(std::string name);
	Channel(const Channel& other);
    ~Channel();
	Channel& operator=(const Channel& other);

	std::string				get_name() {return _name;};
	std::vector<Client *>	get_users() { return _users; };
	std::string 			get_topic() { return _topic; }

	void					add_user(Client *client);
	bool					send_message(std::string sender, std::vector<std::string> &params);
private:
    std::string _name;
    std::string _topic;
    std::vector<Client *> _users;
};


#endif //FT_IRC_CHANNEL_HPP
