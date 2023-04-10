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
	_version = "DEV.1";
	_user_modes = "io";
	_channel_modes = "";

    while (true) {
        listening_fd = _CreateSocket();
        if (listening_fd == -1)
            break;

        _WaitForConnection(listening_fd);
    }
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

    return listening_fd;
}

void TcpListener::_disconnect_client(Client &client, std::string msg)
{
	// send ERROR message to client
	MessageHandler::HandleMessage(client.get_fd(), "ERROR :Closing link: " + msg + "\r\n");

	close(client.get_fd());
	for (int i=0; i < _nfds; i++) {
		if (_fds[i].fd == client.get_fd()) {
			_fds[i] = _fds[_nfds - 1];
			break;
		}
	}

	// if user is in a channel, notify that he leaves and remove him from list
	std::map<std::string, Channel *>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		bool erased = false;
		Channel* chan = it->second; // pour la lisibilité
		if (chan->is_user_in_channel(client.get_fd())) {
			client.leave_channel(*chan);
			if (chan->get_users().empty()) { // if last, delete channel
				it = _channels.erase(it); // set iterator to next channel
				erased = true;
			}
			else { // send quit to remaining channel users
				chan->send_to_users(client.get_nick(),
                  ":" + user_id(client.get_nick(), client.get_username()) +" QUIT :Quit: " + msg);
			}}
		if (_channels.begin() == _channels.end())
			break;
		if (!erased)
			it++;
	}
	delete_client(client.get_fd());
	_nfds--;
}

int TcpListener::_handle_new_connection(int listening_fd) {
	struct sockaddr_in  client_addr;
	socklen_t           client_addr_size;
	int                 client_fd = -1;

	client_addr_size = sizeof(client_addr);

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

int TcpListener::_read_data(int fd, char *buf, std::string& buffer)
{
	ssize_t bytes_received = recv(fd, buf, BUF_SIZE, 0);
	if (bytes_received == -1) { // Handle error
		return -1;
	}
	else if (bytes_received == 0) { // Check to see if the connection has been closed by the client
		_disconnect_client(get_client(fd), "Client disconnected");
		return -1;
	}
	buf[bytes_received] = '\0';
	buffer += buf;
	return 1;
}

int TcpListener::_handle_message(int i) {
	char        buffer[BUF_SIZE];
	std::string output;
	Client& client = get_client(_fds[i].fd);

	memset(buffer, 0, BUF_SIZE);
	if (_read_data(_fds[i].fd, buffer, output) == -1)
		return -1;

	// for registration with irssi, send whole message at once for parsing
	if (!client.is_registered() && is_irssi_client(output)) {
		_process_msg(output, client);
		return 0;
	}
	// for everything else, send to buffer to be parsed line by line
	while (!output.empty()) {
		size_t pos = output.find("\r\n");
		if (pos == std::string::npos) { // incomplete msg, keep reading or return on error
			if (_read_data(_fds[i].fd, buffer, output) == -1)
				return -1;
		}
		else {
			// Complete message received, process it
			_process_msg(output.substr(0, pos), client);
			output = output.substr(pos + 2);
		}
	}
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
		if (msg.substr(5, _password.length()) != _password) {
			MessageHandler::numericReply(client.get_fd(), "464", " :Wrong password");
			_disconnect_client(client, "Client disconnected");
			return;
		}
		_skip_line(msg);
	}
	else {
		MessageHandler::numericReply(client.get_fd(), "461", " :Password required");
		_disconnect_client(client, "Client disconnected");
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

	client.set_registered();
}

void TcpListener::_connection(Client &client) {

	std::string nick = client.get_nick();
	std::string username = client.get_username();

	struct tm* local_time = localtime(&_creation_time);
	char datetime_str[20];
	strftime(datetime_str, 20, "%H:%M:%S %b %d %Y", local_time);

	const std::string user_id = user_id(nick, username);

	MessageHandler::HandleMessage(client.get_fd(), RPL_WELCOME(nick));
	MessageHandler::HandleMessage(client.get_fd(), RPL_YOURHOST(nick, this->_version));
	MessageHandler::HandleMessage(client.get_fd(), RPL_CREATED(nick, datetime_str));
	MessageHandler::HandleMessage(client.get_fd(),
								  RPL_MYINFO(nick, this->_version, this->_user_modes, this->_channel_modes));

	motd(client.get_fd(), nick);

	client.set_connected();
}

void TcpListener::_exec_command(Client &client, const std::string& cmd, std::vector<std::string> &params) {
	std::string valid_commands[13] = {
			"JOIN",
			"PING",
			"PRIVMSG",
			"NOTICE",
			"MODE",
			"NICK",
			"USER",
			"motd",
			"OPER",
			"PART",
			"QUIT",
			"TOPIC",
			"kill"
	};

	int idx = 0;

	while (idx < 13) {
		if (cmd == valid_commands[idx])
			break;
		idx++;
	}
	switch (idx + 1) {
		case 1: join(*this, client, params); break;
		case 2: ping(client); break;
		case 3: _handle_msg(client, "PRIVMSG", params); break;
		case 4: _handle_msg(client, "NOTICE", params); break;
		case 5: _mode(*this, client, params); break;
		case 6: client.set_nickname("NICK " + params[0] + "\r\n", *this); break;
		case 7: client.set_userdata("error"); break;
		case 8: motd(client.get_fd(), client.get_nick()); break;
		case 9: oper(*this, client, params); break;
		case 10: _part_channel(client, params[0]); break;
		case 11: _disconnect_client(client, "Client Disconnected"); break;
		case 12: topic(*this, client, params); break;
    	case 13: kill(*this, client, params); break;
	}
}


void TcpListener::_process_msg(const std::string& msg, Client &client) {
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

		std::cout << "[CLIENT " << client.get_fd() << ": " + client.get_nick() + "] " + msg << std::endl;
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
        if (client_fd == (*it)->get_fd())
            return **it;
    }
	_exit(42);
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

void TcpListener::_handle_msg(Client &client, std::string type, std::vector<std::string> &params) {
	// CHECK VALIDITY
	if (params.size() < 1) {
		if (type == "PRIVMSG") // error messages for PRIVMSG
			MessageHandler::numericReply(client.get_fd(), "411", ":No recipient given (PRIVMSG)");
		return; }
	else if (params.size() < 2) {
		if (type == "PRIVMSG")
			MessageHandler::numericReply(client.get_fd(), "412", ":No text to send");
		return; }

	Channel *chan = _is_channel(params[0]);

	// REDIRECT MESSAGE DEPENDING ON CONTEXT
	if (chan) // if dest is a channel
		chan->send_to_users(client.get_nick(), prep_message(user_id(client.get_nick(), client.get_username()), type, params));
	else if (!_nickname_available(params[0])) // if dest is a user
		MessageHandler::send_to_client(user_id(client.get_nick(), client.get_username()), type, params, this);
	else if (type == "PRIVMSG") // if target is unknown && it's privmsg, send back error
		MessageHandler::numericReply(client.get_fd(), "401", params[0] + " :No such nick/channel");
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

void TcpListener::_part_channel(Client &client, std::basic_string<char> chan) {
	Channel *channel = _is_channel(chan);

	if (channel) {
		client.leave_channel(*channel);
		if (channel->get_users().size() == 0) { // if last, delete channel
			this->_channels.erase(chan); }
		else { // send part to every user in channel
			std::vector<Client *> users = channel->get_users();
			for (std::vector<Client *>::iterator u_it = users.begin(); u_it != users.end(); u_it++)
			{ // todo: put this in a function and join it with the one in _join_channel
				if ((*u_it)->get_nick() != client.get_nick())
					MessageHandler::HandleMessage((*u_it)->get_fd(), ":" + user_id(client.get_nick(),client.get_username()) + " PART " + chan+ " :Leaving Channel\r\n");
			}
		}
	}
	else
		MessageHandler::HandleMessage(client.get_fd(), ERR_NOSUCHCHANNEL(user_id(client.get_nick(), client.get_username()), chan));
}
