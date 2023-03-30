//
// Created by Lucas Dominique on 3/29/23.
//

#include "../CommandHandler.hpp"


void new_channel(TcpListener &SERV, std::string channel_name) {
	std::cout << "NEW CHANNEL: " << channel_name << std::endl;

	Channel channel(channel_name);
	SERV.add_channel(&channel);
}
//The server receiving the command checks whether or not the client can join the given channel, and processes the request.

// While a client is joined to a channel, they receive all relevant information about that channel including the JOIN,
// PART, KICK, and MODE messages affecting the channel. They receive all PRIVMSG and NOTICE messages sent to the channel,
// and they also receive QUIT messages from other clients joined to the same channel (to let them know those users have
// left the channel and the network).
void join(TcpListener &SERV, Client &client, std::vector<std::string> params) {
	std::string channel_name = params.front();
	std::map<std::string, Channel *> & channels = SERV.get_channels();

	// if channel doesn't exist, create it
	std::map<std::string, Channel *>::iterator it = channels.find(channel_name);
	if (it == channels.end())
		new_channel(SERV, channel_name);

	// if channel exist, check if client already in channel



	SERV.print_channels();
}
