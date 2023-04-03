//
// Created by Lucas Dominique on 3/29/23.
//

#include "../CommandHandler.hpp"


Channel *new_channel(TcpListener &SERV, std::string channel_name, std::string op) {
	std::cout << "NEW CHANNEL: " << channel_name << std::endl;

	Channel* channel = new Channel(channel_name, op);
	SERV.add_channel(channel);
	return channel;
}
//The server receiving the command checks whether or not the client can join the given channel, and processes the request.

// While a client is joined to a channel, they receive all relevant information about that channel including the JOIN,
// PART, KICK, and MODE messages affecting the channel. They receive all PRIVMSG and NOTICE messages sent to the channel,
// and they also receive QUIT messages from other clients joined to the same channel (to let them know those users have
// left the channel and the network).

// A JOIN message with the client as the message <source> and the channel they have joined as the first parameter of the message.
void join(TcpListener &SERV, Client &client, std::vector<std::string> params) {
	Channel *channel;
	std::string channel_name = params.front();
	std::map<std::string, Channel *> & channels = SERV.get_channels();

	// if channel doesn't exist, create it
	std::map<std::string, Channel *>::iterator it = channels.find(channel_name);
	if (it == channels.end()) {
		channel = new_channel(SERV, channel_name, client.get_nick());
	}
	else
		channel = it->second;
	// if channel exist, check if client already in channel
	if (!client.in_channel(channel_name)) {
		client.join_channel(channel);
		MessageHandler::HandleMessage(client.get_fd(), ":" + client.get_nick() + " JOIN " + channel_name + "\r\n");
		if (!channel->get_topic().empty())
			MessageHandler::HandleMessage(client.get_fd(), RPL_TOPIC(client.get_nick(), channel_name, channel->get_topic()));
		names(client, *channel);
		//todo: send ":lucas JOIN :#ft_irc" to all users in channel
	}

	SERV.print_channels();
}
