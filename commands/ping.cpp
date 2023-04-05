//
// Created by Lucas Dominique on 3/29/23.
//

#include "../CommandHandler.hpp"
#include "../MessageHandler.hpp"


void ping(int client_fd) {
	MessageHandler::HandleMessage(client_fd,
								  ":localhost PONG localhost :sbars\r\n");
	std::cout << "PONG" << std::endl;
}
