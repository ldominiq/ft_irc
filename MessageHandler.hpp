//
// Created by Lucas on 23-Feb-23.
//

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <iostream>
#include "TcpListener.hpp"

class MessageHandler {
public:
    static void HandleMessage(int socketId, const std::string &msg);
	static void	numericReply(int client_fd, const std::string& numeric, const std::string& message);
};


#endif //MESSAGEHANDLER_HPP
