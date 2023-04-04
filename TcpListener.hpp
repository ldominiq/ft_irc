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
class Channel;

class TcpListener {
public:
    TcpListener(const std::string& ipAddress, int port, std::string password);
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
	Client&			get_client(std::string	&nick);
	bool 			_nickname_available(std::string &nick);
	std::map<std::string, Channel *>& get_channels() { return _channels; }
	void 			add_channel(Channel *channel);
	void 			print_channels();
	Channel*		_is_channel(std::basic_string<char> chan_name);
	void			_disconnect_client(int client_fd);
private:
    int             _CreateSocket() const;
    void            _WaitForConnection(int listening_fd);
    static void     _handle_error(const char *msg);
	void			_process_msg(const std::string& msg, Client &client);
	void			_registration(std::string msg, Client &client);
	static void		_connection(Client &client);
	int 			_handle_new_connection(int listening_fd);
	int 			_handle_message(int i);
	void			_exec_command(Client &client, const std::string& cmd, std::vector<std::string> &params);
	void			_handle_privmsg(Client &client, std::vector<std::string> &params);
	void			_part_channel(Client &client, std::basic_string<char> chan, const char *reason);

    std::string                 _ipAddress;
	std::string					_password;
    int                         _port;
    std::list<Client *>     	_clients;
	int     					_nfds;
	struct pollfd       		_fds[10];
	std::map<std::string, Channel *>    _channels;
};

#endif