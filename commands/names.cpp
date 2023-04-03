//
// Created by Lucas Dominique on 3/31/23.
//

#include "../CommandHandler.hpp"

void	names(Client &client, Channel &channel) {

	std::vector<Client*> users = channel.get_users();
	std::ostringstream msg;
	for (std::vector<Client*>::iterator it = users.begin(); it != users.end(); ++it) {
		Client* user = *it;
		if (channel.get_operator() == user->get_nick())
			msg << "@" << user->get_nick();
		else
			msg << user->get_nick();

		if (std::distance(it, users.end()) > 1) {
			msg << " "; // Add a space after each nickname, except for the last one
		}
	}
	std::string concatenated_nicks = msg.str();

	MessageHandler::HandleMessage(client.get_fd(), RPL_NAMREPLY(client.get_nick(), channel.get_name(), concatenated_nicks));
	MessageHandler::HandleMessage(client.get_fd(), RPL_ENDOFNAMES(client.get_nick(), channel.get_name()));
}