//
// Created by Lucas Dominique on 4/10/23.
//

#include "../CommandHandler.hpp"

//kill test :not talking enough

void	kill(TcpListener &SERV, Client &client, std::vector<std::string> params) {
	const std::string user_id = user_id(client.get_nick(), client.get_username());
	if (!client.is_operator()) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOPRIVILEGES(user_id));
		return;
	}

	if (params.size() < 2) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NEEDMOREPARAMS(user_id, "kill"));
		return;
	}

	std::string reason = params[1].substr(1);

	if (params.size() > 2) {
		std::vector<std::string>::iterator it = params.begin(); it += 2;
		for (; it != params.end(); it++)
			reason += " " + *it;
	}

	std::string kill_msg = "Killed (" + client.get_nick() + " " + reason + ")";

	if (SERV._nickname_available(params[0])) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOSUCHNICK(user_id, params[0]));
		return;
	}

	Client &user = SERV.get_client(params[0]);
	SERV._disconnect_client(user, kill_msg);
}