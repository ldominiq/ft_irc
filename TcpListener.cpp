//
// Created by Lucas on 22-Feb-23.
//

#include "TcpListener.hpp"
#include "utils.hpp"


TcpListener::TcpListener(const std::string& ipAddress, int port)
: _ipAddress(ipAddress), _port(port) {}

TcpListener::~TcpListener() {}

void TcpListener::Send(int clientSocket, const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void TcpListener::Run() {
    int	listening_fd;

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

void TcpListener::_WaitForConnection(int listening_fd) {
    struct sockaddr_in  client_addr;
    socklen_t           client_addr_size;
    int                 client_fd = -1;
	struct pollfd       fds[10];

    bool    end_server = false;
    int     nfds = 1;
    char    buffer[BUF_SIZE];
    ssize_t bytes_received;

    client_addr_size = sizeof(client_addr);

    /*************************************************************/
    /* Initialize the pollfd structure                           */
    /*************************************************************/
    memset(fds, 0 , sizeof(*fds));

    /*************************************************************/
    /* Set up the initial listening socket                        */
    /*************************************************************/
    fds[0].fd = listening_fd;
    fds[0].events = POLLIN;

    /*************************************************************/
    /* Loop waiting for incoming connects or for incoming data   */
    /* on any of the connected sockets.                          */
    /*************************************************************/
    do {
		print_debug("Waiting on poll()...");
        if (poll(fds, nfds, -1) < 0)
            _handle_error("poll() failed");

        /***********************************************************/
        /* One or more descriptors are readable.  Need to          */
        /* determine which ones they are.                          */
        /***********************************************************/
        for (int i = 0; i < nfds; i++) {
            /*********************************************************/
            /* Loop through to find the descriptors that returned    */
            /* POLLIN and determine whether it's the listening       */
            /* or the active connection.                             */
            /*********************************************************/
            if (fds[i].revents == 0)
                continue;

			/*********************************************************/
			/* listening socket, therefore there is a new connection */
			/*********************************************************/
            if (fds[i].fd == listening_fd) {
                print_debug("Listening socket is readable");

                client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &client_addr_size);
                if (client_fd < 0) {
                    perror("Error on client connecting.");
                    break;
                }

                /*****************************************************/
                /* Add the new incoming connection to the            */
                /* pollfd structure                                  */
                /*****************************************************/
                std::cout << "Accepted new connection from: " << inet_ntoa(client_addr.sin_addr) << " on socket: " << client_fd << std::endl;
                fds[nfds].fd = client_fd;
                fds[nfds].events = POLLIN;
                fds[nfds].revents = 0;
                nfds++;
                this->_clients.push_back(new Client(client_fd));

                std::string welcome = "gigacoolchat v0.1";
				print_debug(welcome);
                MessageHandler::HandleMessage(client_fd, welcome);
            }

            /*********************************************************/
            /* This is not the listening socket, therefore an        */
            /* existing connection must be readable                  */
            /*********************************************************/
            else {
                std::cout << "Descriptor: " << fds[i].fd << " is readable" << std::endl;

                memset(buffer, 0, BUF_SIZE);

                //Wait for client to send data
                bytes_received = recv(fds[i].fd, buffer, BUF_SIZE, 0);

                if (bytes_received == -1) {
                    std::cout << "Connection issue" << std::endl;
                    break;
                }

                /*****************************************************/
                /* Check to see if the connection has been           */
                /* closed by the client                              */
                /*****************************************************/
                if (bytes_received == 0) {
                    std::cout << "Client disconnected" << std::endl;
                    close(fds[i].fd);
                    fds[i] = fds[nfds - 1];
					delete_client(fds[i].fd);
                    nfds--;
                    break;
                }
                std::cout << bytes_received << " bytes received." << std::endl;
                std::cout << "Message received: " << std::endl
				<< buffer << std::endl;
				_process_msg(buffer, get_client(fds[i].fd));
            } /* End of existing connection is readable             */
        } /* End of loop through pollable descriptors              */
    } while (!end_server); /* End of serving running.    */

    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }
}



void TcpListener::_process_msg(std::string msg, Client	&client)
{
	if (!client.get_status()) // CONNECTION PROCEDURE
	{
//			char *current_ptr = (char *) msg.c_str() + i;
			if (msg.find("CAP") == 0 || msg.find("PASS") == 0) {
				_skip_line(msg); }
			if (msg.find("NICK") == 0){
					if (!client.set_nickname(msg, this->_clients))
						_handle_error("other nickname error");
				_skip_line(msg);
			}
			else if (strncmp("USER", current_ptr, 4) == 0) {
				// DO SOMETHING
			}
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

std::string TcpListener::get_ip()
{
	return (this->_ipAddress);
}

