//
// Created by Lucas Dominique on 3/29/23.
//

#include "../CommandHandler.hpp"
#include "../MessageHandler.hpp"


void ping(Client &client) {
	MessageHandler::HandleMessage(client.get_fd(),
								  ":localhost PONG localhost :localhost\r\n");
}
