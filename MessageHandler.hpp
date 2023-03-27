//
// Created by Lucas on 23-Feb-23.
//

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include "TcpListener.hpp"
#include <iostream>

class MessageHandler {
public:
    static void HandleMessage(int socketId, const std::string &msg);
	static void	numericReply(int client_fd, std::string numeric, std::string message);
};


#endif //MESSAGEHANDLER_HPP
