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
#include <list>

#include "MessageHandler.hpp"
#include "Modifier.hpp"
#include "Client.hpp"
#include "utils.hpp"

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
	void					delete_client(int	client_fd);
	Client&					get_client(int client_fd);
	static bool 			_nickname_available(std::string &nick);
private:
    int             _CreateSocket() const;
    void            _WaitForConnection(int listening_fd);
    static void     _handle_error(const char *msg);
	void			_process_msg(std::string msg, Client &client);

    std::string                 _ipAddress = "127.0.0.1";
    int                         _port;
    std::list<Client *>     _clients;
};

#endif