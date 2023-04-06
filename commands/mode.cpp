//
// Created by Lucas Dominique on 4/5/23.
//

#include "../CommandHandler.hpp"

void _mode(TcpListener &SERV, Client &client, std::vector<std::string> params)
{
	if (params.size() < 2) {
		const std::string user_id = user_id(client.get_nick(), client.get_username());
		MessageHandler::HandleMessage(client.get_fd(), ERR_NEEDMOREPARAMS(user_id, "MODE"));
		return;
	}
	const std::string user_id = user_id(client.get_nick(), client.get_username());

	std::string target = params[0];
	std::string mode_changes = params[1];

	if ((mode_changes.find('o') != std::string::npos) && !client.is_operator()) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOPRIVILEGES(user_id));
		return;
	}

	if (target != client.get_nick()) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_USERSDONTMATCH(user_id));
		return;
	}

	// Check if target is a channel or a user
	bool is_channel = false;
	if (target[0] == '&') {
		is_channel = true;
	}

	// Check if target exists
	if (is_channel) {
		if (SERV._is_channel(target) != NULL) {
			MessageHandler::HandleMessage(client.get_fd(), ERR_NOSUCHCHANNEL(user_id, target));
			return;
		}
	} else {
		if (SERV._nickname_available(target)) {
			MessageHandler::HandleMessage(client.get_fd(), ERR_NOSUCHNICK(user_id, target));
			return;
		}
	}

	// Construct and send response message
	std::string response = ":" + user_id + " MODE " + target + " :" + mode_changes + "\r\n";
	MessageHandler::HandleMessage(client.get_fd(), response);
}