//
// Created by Lucas Dominique on 3/29/23.
//

#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void ping(int client_fd, std::vector<std::string> params) {
	MessageHandler::HandleMessage(client_fd,
								  ":localhost PONG localhost :sbars\r\n");
	std::cout << "PONG" << std::endl;
}

void _mode(int fd, std::vector<std::string> params)
{
	MessageHandler::HandleMessage(fd,
								  ":sbars MODE sbars :+i\r\n");
	std::cout << "mode" << std::endl;
}
