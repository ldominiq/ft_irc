//
// Created by Lucas Dominique on 3/29/23.
//

#include "CommandHandler.hpp"
#include "MessageHandler.hpp"

void ping(int client_fd, std::vector<std::string> params) {
	MessageHandler::HandleMessage(client_fd,
								  ":127.0.0.1 PONG");
	std::cout << "PONG" << std::endl;
}