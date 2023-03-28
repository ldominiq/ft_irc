//
// Created by Lucas Dominique on 3/28/23.
//

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "TcpListener.hpp"
#include "Client.hpp"

class Client;

typedef void(*CommandHandler)(Client &, int, const std::string);
std::map<std::string, CommandHandler> commandMap;




void handleJoin(Client &client, int argc, const std::string argv) { /* ... */ }
void handleNick(Client &client, int argc, const std::string argv) { /* ... */ }
void handleQuit(Client &client, int argc, const std::string argv) { /* ... */ }
void handlePing(Client &client, int argc, const std::string argv) {
	//MessageHandler::HandleMessage(client.get_fd(), ":127.0.0.1 PONG " + client.get_hostname() + " :" + client.get_nick());
}

void populateCommands() {
	commandMap["JOIN"] = &handleJoin;
	commandMap["NICK"] = &handleNick;
	commandMap["QUIT"] = &handleQuit;
	commandMap["PING"] = &handlePing;
};

#endif