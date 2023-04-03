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
    Channel(std::string name, std::string op);
	Channel(const Channel& other);
    ~Channel();
	Channel& operator=(const Channel& other);

	std::string				get_name() {return _name;};
	std::vector<Client *>	get_users() { return _users; };
	std::string 			get_topic() { return _topic; }
	std::string 			get_operator() { return _operator; }

	void					add_user(Client *client);
	void					add_operator(std::string nick);
	bool					send_message(std::string sender, std::vector<std::string> &params);
private:
    std::string _name;
    std::string _topic;
    std::vector<Client *> _users;
	std::string _operator;
};


#endif //FT_IRC_CHANNEL_HPP
