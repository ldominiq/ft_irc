//
// Created by Lucas on 24-Feb-23.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP


#include <string>
#include <vector>
#include <iostream>

class Client;

class Channel {
public:
    Channel(std::string name);
    ~Channel();

	std::string get_name() {return _name;};
	bool		send_message(std::string nick, std::string &msg);
private:
    std::string _name;
    std::string _topic;
    std::vector<Client *> _users;
};


#endif //FT_IRC_CHANNEL_HPP
