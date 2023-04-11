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

	std::string modes_supported;
	std::string modes_not_supported;
	std::string user_modes = SERV.get_user_modes();

	char mode_sign = mode_changes[0];
	if (mode_sign == '+' || mode_sign == '-') {
		modes_supported = mode_sign;
		for (std::string::size_type i = 1; i < mode_changes.length(); ++i) {
			char c = mode_changes[i];
			if (user_modes.find(c) != std::string::npos) {
				modes_supported += c;
			} else {
				modes_not_supported += c;
			}
		}

		// Construct and send response message
		if (!modes_supported.empty() && modes_supported.size() != 1) {
			std::string response = ":" + user_id + " MODE " + target + " :" + modes_supported + "\r\n";
			MessageHandler::HandleMessage(client.get_fd(), response);
		}
		if (!modes_not_supported.empty())
			MessageHandler::HandleMessage(client.get_fd(), ERR_UMODEUNKNOWNFLAG(user_id, modes_not_supported));
	}
}
