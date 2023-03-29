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

#include "Modifier.hpp"
#include "Numeric_replies.hpp"
#include "utils.hpp"
#include "MessageHandler.hpp"
#include "Client.hpp"

#define BUF_SIZE 1024

class Client;

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
	void			delete_client(int	client_fd);
	Client&			get_client(int client_fd);
	bool 			_nickname_available(std::string &nick);
private:
    int             _CreateSocket() const;
    void            _WaitForConnection(int listening_fd);
    static void     _handle_error(const char *msg);
	void			_process_msg(const std::string& msg, Client &client);
	void			_disconnect_client(int client_fd);
	void			_registration(std::string msg, Client &client);
	static void		_connection(Client &client);
	int 			_handle_new_connection(int listening_fd);
	int 			_handle_message(int i);
	static void		_exec_command(Client &client, const std::string& cmd, const std::vector<std::string>& params);

    std::string                 _ipAddress;
    int                         _port;
    std::list<Client *>     	_clients;
	int     					_nfds;
	struct pollfd       		_fds[10];
	std::string 				_commands[20];

	void _handle_join(Client &client, std::vector<std::string> &params);

	void _handle_privmsg(Client &client, std::vector<std::string> &vector1);
};

#endif