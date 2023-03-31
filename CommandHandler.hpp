//
// Created by Lucas Dominique on 3/28/23.
//

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <vector>

//void join(int client_fd, std::vector<std::string> params);
//void nick(int client_fd, std::vector<std::string> params);
void ping(int client_fd, std::vector<std::string> params);
void _mode(int fd, std::vector<std::string> params);
//void privmsg(int client_fd, std::vector<std::string> params);

#endif