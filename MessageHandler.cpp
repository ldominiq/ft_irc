//
// Created by Lucas on 23-Feb-23.
//

#include "MessageHandler.hpp"

void MessageHandler::HandleMessage(int socketId, const std::string &msg) {
    TcpListener::Send(socketId, msg);
}

void MessageHandler::numericReply(int client_fd, std::string numeric, std::string message)
{
	// todo: check that <client> prefix is always needed
	std::string reply = ":127.0.0.1 " + numeric + "\r\n" + message + "\r\n";
	std::cout << reply << std::endl;
	TcpListener::Send(client_fd, reply);
}
