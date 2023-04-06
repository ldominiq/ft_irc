//
// Created by Lucas Dominique on 4/5/23.
//

#include "../CommandHandler.hpp"

void	oper(TcpListener &SERV, Client &client, std::vector<std::string> params) {
	if (client.is_operator()) {
		return;
	}
	const std::string user_id = user_id(client.get_nick(), client.get_username());
	if (params.size() != 2) {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NEEDMOREPARAMS(user_id, "OPER"));
		return;
	}
	if (params[0] != "god") {
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOOPERHOST(user_id));
		return;
	}
	if (params[1] != "dog") {
		MessageHandler::HandleMessage(client.get_fd(), ERR_PASSWDMISMATCH(user_id));
		return;
	}
	client.set_operator();
	MessageHandler::HandleMessage(client.get_fd(), RPL_YOUREOPER(user_id));

	std::vector<std::string> mode;
	mode.push_back(client.get_nick());
	mode.push_back("+o");
	_mode(SERV, client, mode);
}