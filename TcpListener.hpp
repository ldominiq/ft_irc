//
// Created by Lucas on 22-Feb-23.
//

#ifndef TCPLISTENER_HPP
#define TCPLISTENER_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <map>

#include "MessageHandler.hpp"
#include "Modifier.hpp"
#include "Client.hpp"

#define BUF_SIZE 1024

class MessageHandler;

class TcpListener {
public:
    TcpListener(const std::string& ipAddress, int port);
    virtual ~TcpListener();

    // Send a message to the specified client
    static void Send(int clientSocket, const std::string& msg);

    // Main processing loop
    void Run();

	template<typename T>
	void	print_debug(T message) {
		std::cout << "===DEBUG===: " << message << std::endl;
	}

private:
    int             _CreateSocket() const;
    void            _WaitForConnection(int listening_fd, struct pollfd *fds);
    static void     _handle_error(const char *msg);
	void			_process_msg(std::string msg);

    std::string                 _ipAddress;
    int                         _port;
    std::map<int, Client *>     _clients;
};

#endif //TCPLISTENER_HPP
