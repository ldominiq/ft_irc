//
// Created by Lucas on 23-Feb-23.
//
#include "MessageHandler.hpp"

void MessageHandler::HandleMessage(int socketId, const std::string &msg) {
    TcpListener::Send(socketId, msg);
}

void MessageHandler::numericReply(int client_fd, std::string numeric, std::string message)
{
	std::string reply = ":127.0.0.1 " + numeric + " " + message + "\r\n";
	TcpListener::Send(client_fd, reply);
}

void MessageHandler::send_to_client(std::string sender, std::vector<std::string> &params, TcpListener *SERV)
{
	std::string message = params[1];
	// concatenate params 1 to n into string message
	if (params.size() > 2) {
		for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++)
			message += *it;
	}

	std::string trimmed = message.substr(1); // REMOVE the ":" prefix
	// FIND CLIENT USING NICKNAME
	Client&	client = SERV->get_client(params[0]);
	std::cout << client.get_fd() << std::endl;
	// SEND REFORMATED MESSAGE TO CLIENT
	std::cout << "message " + message + "$" << std::endl;
	std::string reform = ":" + sender + " PRIVMSG " + client.get_nick() + " " + message + "\r\n";

	std::cout << reform << std::endl;
	MessageHandler::HandleMessage(client.get_fd(), reform);
}
