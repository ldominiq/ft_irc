//
// Created by Lucas Dominique on 4/5/23.
//

#include "../CommandHandler.hpp"

void	motd(int client_fd, std::string nick) {
	MessageHandler::HandleMessage(client_fd, RPL_MOTDSTART(nick));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "- WELCOME to sbars x ldominiq IRC serv"));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "-  O"));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "-  |"));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "- / \\"));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "-  |"));
	MessageHandler::HandleMessage(client_fd, RPL_MOTD(nick, "- / \\"));
	MessageHandler::HandleMessage(client_fd, RPL_ENDOFMOTD(nick));
}
