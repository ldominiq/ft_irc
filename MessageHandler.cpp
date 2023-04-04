//
// Created by Lucas on 23-Feb-23.
//
#include "MessageHandler.hpp"

void MessageHandler::HandleMessage(int socketId, const std::string &msg) {
    TcpListener::Send(socketId, msg);
}

void MessageHandler::numericReply(int client_fd, const std::string& numeric, const std::string& message)
{
	std::string reply = ":127.0.0.1 " + numeric + " " + message + "\r\n";
	std::cout << reply << std::endl;
	TcpListener::Send(client_fd, reply);
}

void MessageHandler::send_to_client(std::string sender, std::vector<std::string> &params, TcpListener *SERV)
{
	std::string message = params[1];
	std::cout << "message: " << message << std::endl;
	// concatenate params 2 (because is already in message) to n into string msg if it's fragmented
	if (params.size() > 2) {
		std::vector<std::string>::iterator it = params.begin(); it += 2;
		for (; it != params.end(); it++)
			message += " " + *it;
	}
	// FIND CLIENT USING NICKNAME
	Client&	client = SERV->get_client(params[0]);
	std::cout << client.get_fd() << std::endl;
	// SEND REFORMATED MESSAGE TO CLIENT
	std::string reform = ":" + sender + " PRIVMSG " + client.get_nick() + " " + message + "\r\n";
	MessageHandler::HandleMessage(client.get_fd(), reform);
}
