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
	TcpListener::Send(client_fd, reply);
}

void MessageHandler::send_to_client(std::string sender, std::string type, std::vector<std::string> &params, TcpListener *SERV)
{
	// prep message
	std::string message = prep_message(sender, type, params) + "\r\n";

	// find client (we need his fd, but only have his name
	Client&	client = SERV->get_client(params[0]);

	//send to client
	MessageHandler::HandleMessage(client.get_fd(), message);
}
