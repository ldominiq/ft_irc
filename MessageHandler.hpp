//
// Created by Lucas on 23-Feb-23.
//

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

#include <iostream>
#include "TcpListener.hpp"

class TcpListener;

class MessageHandler {
public:
    static void HandleMessage(int socketId, const std::string &msg);
	static void	numericReply(int client_fd, std::string numeric, std::string message);
	static void send_to_client(std::string sender, std::vector<std::string> &params, TcpListener *SERV);
};


#endif //MESSAGEHANDLER_HPP
