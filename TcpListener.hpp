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
#include <ctime>

#include "Modifier.hpp"
#include "Numeric_replies.hpp"
#include "utils.hpp"
#include "MessageHandler.hpp"
#include "Client.hpp"

#define BUF_SIZE 1024
#define PRIVMSG 0
#define NOTICE 1

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

	void			delete_client(int	client_fd);
	Client&			get_client(int client_fd);
	Client&			get_client(std::string	&nick);
	bool 			_nickname_available(std::string &nick);
	std::map<std::string, Channel *>& get_channels() { return _channels; }
	void 			add_channel(Channel *channel);
	void 			print_channels();
	Channel*		_is_channel(std::basic_string<char> chan_name);
	void			_disconnect_client(Client &client, std::string msg);
private:
    int             _CreateSocket() const;
    void            _WaitForConnection(int listening_fd);
    static void     _handle_error(const char *msg);
	void			_process_msg(const std::string& msg, Client &client);
	void			_registration(std::string msg, Client &client);
	void			_connection(Client &client);
	int 			_handle_new_connection(int listening_fd);
	int 			_read_data(int fd, char *buf, std::string& buffer);
	int 			_handle_message(int i);
	void			_exec_command(Client &client, const std::string& cmd, std::vector<std::string> &params);
	void _handle_msg(Client &client, std::string type, std::vector<std::string> &params);
	void			_part_channel(Client &client,  std::string chan);

    std::string                 		_ipAddress;
	std::string							_password;
			int                         _port;
    std::list<Client *>     			_clients;
	int     							_nfds;
	struct pollfd       				_fds[10];
	std::string 						_commands[20];
	std::map<std::string, Channel *>    _channels;
	time_t								_creation_time;
	std::string 						_version;
	std::string 						_user_modes;
	std::string 						_channel_modes;
};

#endif