//
// Created by Samuel Bars on 3/22/23.
//

#include "TcpListener.hpp"

void	_skip_line(std::string &msg, int &i)
{
	while (msg[i] != '\n' && msg[i] != '\r' && msg[i] != msg.back())
		i++;
}

void send_numeric_reply(int client_fd, int numeric, const std::string &message)
{
	std::string reply = ":" + std::to_string(numeric) + " " + message + "\r";
	TcpListener::Send(client_fd, reply);
}