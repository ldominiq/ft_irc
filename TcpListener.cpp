//
// Created by Lucas on 22-Feb-23.
//
#include "TcpListener.hpp"
#include "CommandHandler.hpp"

TcpListener::TcpListener(const std::string& ipAddress, int port, std::string password)
: _ipAddress(ipAddress), _password(password), _port(port) {}

TcpListener::~TcpListener() {}

void TcpListener::Send(int clientSocket, const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

void TcpListener::Run() {
    int	listening_fd;

	_creation_time = time(nullptr);

    while (true) {
        listening_fd = _CreateSocket();
        if (listening_fd == -1)
            break;

        _WaitForConnection(listening_fd);
    }
    std::cout << "Goodbye..." << std::endl;
}

int TcpListener::_CreateSocket() const {
    int                 listening_fd;
    const int           enable = 1;
    struct sockaddr_in  server_addr;

    /*************************************************************/
    /* Create an AF_INET stream socket to receive incoming      */
    /* connections on                                            */
    /*************************************************************/
    listening_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_fd < 0)
        _handle_error("Error establishing connection.");

    /*************************************************************/
    /* Allow socket descriptor to be reuseable                   */
    /*************************************************************/
    if (setsockopt(listening_fd, SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0)
        _handle_error("Error setting socket options.");

    /*************************************************************/
    /* Set socket to be nonblocking. All the sockets for      */
    /* the incoming connections will also be nonblocking since   */
    /* they will inherit that state from the listening socket.   */
    /*************************************************************/
    if (fcntl(listening_fd, F_SETFL, O_NONBLOCK))
        _handle_error("fcntl() failed.");

    std::cout << "Server Socket connection created..." << std::endl;

    /*************************************************************/
    /* Bind the socket                                           */
    /*************************************************************/
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;   /* address family: AF_INET = ipv4 */
    server_addr.sin_port = htons(_port);    /* port in network byte order, htons() = host to network short */
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);    /* internet address -> address in network byte order, INADDR_ANY = any address */

    if (bind(listening_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        _handle_error("Error binding socket.");

    /*************************************************************/
    /* Set the listen backlog                                   */
    /*************************************************************/
    if (listen(listening_fd, 1) < 0) {
        perror("Error while listening.");
        return -1;
    }

    //std::cout << blue << "| listen() is OK ! |" << def << std::endl;
    std::cout << "Looking for clients..." << std::endl;

    return listening_fd;
}

void TcpListener::_disconnect_client(int client_fd) {
	std::cout << "Client disconnected" << std::endl;
	close(client_fd);
	for (int i=0; i < _nfds; i++) {
		if (_fds[i].fd == client_fd) {
			_fds[i] = _fds[_nfds - 1];
			break;
		}
	}

	Client&	client = get_client(client_fd);
	// if user is in a channel, notify that he leaves and remove him from list
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second->is_user_in_channel(client_fd)) {
			std::cout << "channel: " << it->first << " - user: " << client.get_nick() << " is leaving" << std::endl;
			_part_channel(client, it->first, "Leaving channel");
			// todo: send message to all channel members, but replace command below with a call to PART with "Leaving channel" as reason
//			it->second->send_message(client.get_nick() + "!" + client.get_username() + "@" + client.get_username(), msg);
		}
	}
	delete_client(client_fd);
	_nfds--;
}

int TcpListener::_handle_new_connection(int listening_fd) {

	struct sockaddr_in  client_addr;
	socklen_t           client_addr_size;
	int                 client_fd = -1;

	client_addr_size = sizeof(client_addr);

	print_debug("Listening socket is readable");

	client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_fd < 0) {
		perror("Error on client connecting.");
		return -1;
	}

	// Add the new incoming connection to the pollfd structure
	std::string hostname(inet_ntoa(client_addr.sin_addr));
	std::cout << "Accepted new connection from: " << hostname << " on socket: " << client_fd << std::endl;
	_fds[_nfds].fd = client_fd;
	_fds[_nfds].events = POLLIN;
	_fds[_nfds].revents = 0;
	_nfds++;

	this->_clients.push_back(new Client(client_fd, hostname));
	return 0;
}

int TcpListener::_handle_message(int i) {

	char    buffer[BUF_SIZE];
	ssize_t bytes_received;

	std::cout << "Descriptor: " << _fds[i].fd << " is readable" << std::endl;

	memset(buffer, 0, BUF_SIZE);

	//Wait for client to send data
	bytes_received = recv(_fds[i].fd, buffer, BUF_SIZE, 0);

	if (bytes_received == -1) {
		std::cout << "Connection issue" << std::endl;
		return -1;
	}

	// Check to see if the connection has been closed by the client
	if (bytes_received == 0) {
		_disconnect_client(_fds[i].fd);
		return -1;
	}
	std::cout << bytes_received << " bytes received." << std::endl;
	std::cout << "Message received: " << std::endl
			  << buffer << std::endl;
	_process_msg(buffer, get_client(_fds[i].fd));

	return 0;
}

void TcpListener::_WaitForConnection(int listening_fd) {
    bool    end_server = false;
    _nfds = 1;

    // Initialize the pollfd structure
    memset(_fds, 0, sizeof(*_fds));

    // Set up the initial listening socket
    _fds[0].fd = listening_fd;
    _fds[0].events = POLLIN;

    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    do {
		print_debug("Waiting on poll()...");
        if (poll(_fds, _nfds, -1) < 0)
            _handle_error("poll() failed");

        // One or more descriptors are readable. New connection or new message
        for (int i = 0; i < _nfds; i++) {
            if (_fds[i].revents == 0)
                continue;

			// listening socket, therefore there is a new connection
			if (_fds[i].fd == listening_fd) {
				if (_handle_new_connection(listening_fd) == -1)
					break;
			}
			else {
                if (_handle_message(i) == -1)
					break;
			}
        }
    } while (!end_server);

    for (int i = 0; i < _nfds; i++) {
        if (_fds[i].fd >= 0)
            close(_fds[i].fd);
    }
}

void TcpListener::_registration(std::string msg, Client &client) {
	if (msg.find("CAP") == 0) {
		_skip_line(msg); }
	if (msg.find("PASS") == 0)
	{
		std::cout << msg.substr(5, 12) << std::endl;
		if (msg.substr(5, _password.length()) != _password) {
			MessageHandler::numericReply(client.get_fd(), "464", " :Wrong password");
			_disconnect_client(client.get_fd());
			return;
		}
		_skip_line(msg);
	}
	else {
		MessageHandler::numericReply(client.get_fd(), "461", " :Password required");
		_disconnect_client(client.get_fd());
		return;
	}
	if (msg.find("NICK") == 0){
		if (!client.set_nickname(msg, *this))
			return ;
		_skip_line(msg);
	}
	if (msg.find("USER") == 0) {
		if (!client.set_userdata(msg))
			return ;
		_skip_line(msg);
	}
	client.get_infos();

	client.set_registered();
}

void TcpListener::_connection(Client &client) {

	std::string nick = client.get_nick();
	std::string username = client.get_username();

	struct tm* local_time = localtime(&_creation_time);
	char datetime_str[20];
	strftime(datetime_str, 20, "%H:%M:%S %b %d %Y", local_time);

	std::string msg = RPL_WELCOME(user_id(nick, username), nick) +
			RPL_YOURHOST(nick) +
			RPL_CREATED(datetime_str) +
			RPL_MYINFO();
	MessageHandler::HandleMessage(client.get_fd(), msg);

	motd(client.get_fd(), nick);

	client.set_connected();

}

void TcpListener::_exec_command(Client &client, const std::string& cmd, std::vector<std::string> &params)
{

	std::string valid_commands[8] = {
			"JOIN",
			"PING",
			"PRIVMSG",
			"MODE",
			"NICK",
			"USER",
      		"motd",
			  "OPER"
			//"PART"
	};

	int idx = 0;

	while (idx < 7) {
		if (cmd == valid_commands[idx])
			break;
		idx++;
	}
	switch (idx + 1) {
		case 1: join(*this, client, params); break;
		case 2: ping(client.get_fd()); break;
		case 3: _handle_privmsg(client, params); break;
		case 4: _mode(*this, client, params); break;
		case 5: client.set_nickname("NICK " + params[0] + "\r\n", *this); break;
		case 6: client.set_userdata("USER " + params[0] + " " + params[1] + " " + params[2] + " " + params[3] + "\r\n"); break;
    	case 7: motd(client.get_fd(), client.get_nick()); break;
		case 8: oper(*this, client, params); break;
		//case 7: part
	}
}

void TcpListener::_process_msg(const std::string& msg, Client	&client)
{
	if (!client.is_registered()) // connection procedure
		_registration(msg, client);
	if (!client.is_connected())
		_connection(client);
	else // all commands after registration
	{
		std::istringstream iss(msg);
		std::string cmd;
		iss >> cmd;

		std::vector<std::string> params;
		for (std::string param; iss >> param;)
			params.push_back(param);

		_exec_command(client, cmd, params);
	}
}

void TcpListener::_handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

bool TcpListener::_nickname_available(std::string &nick)
{
	std::list<Client *>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++){
		if (!nick.empty() && nick == ((*it))->get_nick())
			return (false);
	}
	return (true);
}

Channel * TcpListener::_is_channel(std::basic_string<char> chan_name)
{
	if (chan_name[0] != '&')
		return nullptr;

	std::map<std::string, Channel *>::iterator it = this->_channels.find(chan_name);
	if (it == this->_channels.end())
		return nullptr;
	else
		return it->second;
}

void TcpListener::delete_client(int client_fd) {
    std::list<Client *>::iterator it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++){
		if (((*it)->get_fd() == client_fd))
			this->_clients.erase(it);
	}
}

Client& TcpListener::get_client(int client_fd) {
    std::list<Client*>::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if ((*it)->get_fd() == client_fd)
            return **it;
    }

    throw std::runtime_error("Client not found"); // or return some default value instead of throwing an exception
}

Client &TcpListener::get_client(std::string &nick)
{
	std::list<Client *>::iterator it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++){
		if (!nick.empty() && nick == ((*it))->get_nick())
			return **it;
	}
	throw std::runtime_error("Client not found"); // or return some default value instead of throwing an exception
}

void TcpListener::_handle_privmsg(Client &client, std::vector<std::string> &params)
{
	if (params.size() < 1) {
		MessageHandler::numericReply(client.get_fd(), "411", ":No recipient given (PRIVMSG)");
		return; }
	else if (params.size() < 2) {
		MessageHandler::numericReply(client.get_fd(), "412", ":No text to send");
		return; }

	Channel *chan = _is_channel(params[0]);
	if (chan){
		chan->send_message(client.get_nick(), params);
	}
	else if (!_nickname_available(params[0]))
		MessageHandler::send_to_client(client.get_nick(), params, this);
	else {
		MessageHandler::numericReply(client.get_fd(), "401", params[0] + " :No such nick/channel");}
}

void TcpListener::add_channel(Channel *channel) {
	this->_channels.insert(std::pair<std::string, Channel *>(channel->get_name(), channel));
}

void TcpListener::print_channels() {
	std::map<std::string, Channel *> channels = this->get_channels();

	std::map<std::string, Channel *>::iterator it = channels.begin();
	std::cout << "CHANNELS LIST:" << std::endl;
	while (it != channels.end()) {
		std::vector<Client *> users = it->second->get_users();
		std::cout << "USERS IN CHANNEL:" << it->first << std::endl;
		for (std::vector<Client*>::iterator it2 = users.begin(); it2 != users.end(); ++it2) {
			Client* user = *it2;
			std::cout << user->get_nick() << " ";
		}
		std::cout << std::endl;
		it++;
	}
}

void TcpListener::_part_channel(Client &client, std::basic_string<char> chan, const char *reason) {
	Channel *channel = _is_channel(chan);
	if (channel) {
		client.leave_channel(chan);
		print_channels();
		if (channel->get_users().size() == 0) { // if last, delete channel
			this->_channels.erase(chan); }
		else { // send part to every user in channel
			std::vector<Client *> users = channel->get_users();
			for (std::vector<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
			{ // todo: put this in a function and join it with the one in _join_channel
				if ((*u_it)->get_nick() != client.get_nick())
					MessageHandler::HandleMessage((*u_it)->get_fd(), ":" + user_id(client.get_nick(),client.get_username()) + " PART " + chan + " :" + reason + "\r\n");
			}
		}
	}
}
