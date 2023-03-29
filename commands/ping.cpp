//
// Created by Lucas Dominique on 3/29/23.
//

#include "../CommandHandler.hpp"
#include "../MessageHandler.hpp"

void ping(Client &client, std::vector<std::string> params) {
	MessageHandler::HandleMessage(client.get_fd(),
								  ":127.0.0.1 PONG " + client.get_hostname() + " :" + client.get_nick());
	std::cout << "PONG" << std::endl;
}