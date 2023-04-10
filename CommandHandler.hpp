//
// Created by Lucas Dominique on 3/28/23.
//

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <vector>
#include <map>
#include "Client.hpp"

void 	join(TcpListener &SERV, Client &client, std::vector<std::string> params);
//void nick(int client_fd, std::vector<std::string> params);
void 	ping(int client_fd);
void 	_mode(TcpListener &SERV, Client &client, std::vector<std::string> params);
//void privmsg(int client_fd, std::vector<std::string> params);
void	names(Client &client, Channel &channel);
void	motd(int client_fd, std::string nick);
void 	oper(TcpListener &SERV, Client &client, std::vector<std::string> params);
void	topic(TcpListener &SERV, Client &client, std::vector<std::string> params);
void 	kill(TcpListener &SERV, Client &client, std::vector<std::string> params);

#endif