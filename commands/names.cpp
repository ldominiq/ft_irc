//
// Created by Lucas Dominique on 3/31/23.
//

#include "../CommandHandler.hpp"

void	names(Client &client, Channel &channel) {

	std::vector<Client*> users = channel.get_users();
	std::string msg;
	for (std::vector<Client*>::iterator it = users.begin(); it != users.end(); ++it) {
		Client* user = *it;
		msg += user->get_nick() + " ";
	}

	MessageHandler::HandleMessage(client.get_fd(), RPL_NAMREPLY(client.get_nick(), channel.get_name(), msg));
	MessageHandler::HandleMessage(client.get_fd(), RPL_ENDOFNAMES(client.get_nick(), channel.get_name()));
}