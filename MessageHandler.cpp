//
// Created by Lucas on 23-Feb-23.
//

#include "MessageHandler.hpp"

void MessageHandler::HandleMessage(int socketId, const std::string &msg) {
    TcpListener::Send(socketId, msg);
}

void MessageHandler::numericReply(int client_fd, int numeric, std::string message)
{
	std::string reply = ":127.0.0.1 " + std::to_string(numeric) + " " + message;
	TcpListener::Send(client_fd, reply);
}
