//
// Created by Lucas on 22-Feb-23.
//

#include "TcpListener.hpp"


TcpListener::TcpListener(const std::string& ipAddress, int port)
: _ipAddress(ipAddress), _port(port) {

}

TcpListener::~TcpListener() {

}

void TcpListener::Send(int clientSocket, const std::string& msg) {
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void TcpListener::Run() {
    char buffer[BUF_SIZE];

    while (true) {
        int listening_fd = _CreateSocket();
        if (listening_fd == -1)
            break;

        int client_fd = _WaitForConnection(listening_fd);
        if (client_fd != -1) {
            close(listening_fd);

            this->_clients.insert(std::pair<int, Client *>(client_fd, new Client(client_fd)));


//            memset(buffer, 0, BUF_SIZE);

            // Wait for client to send data
//            ssize_t bytes_received;
//
//            do {
//                bytes_received = recv(client_fd, buffer, BUF_SIZE, 0);
//
//                if (bytes_received == -1) {
//                    std::cout << "Connection issue" << std::endl;
//                    break;
//                }
//                else if (bytes_received == 0) {
//                    std::cout << "Client disconnected" << std::endl;
//                    break;
//                }
//
//                std::cout << "Message received: " << buffer << std::endl;
//
//                //if (strncmp("CAP ", buffer, 4) != 0)
//                    //MessageHandler::HandleMessage(client_fd, std::string(buffer, 0, bytes_received));
//
//            } while (bytes_received > 0);
//
//            close(client_fd);
        }
    }
    //std::cout << "Server Socket connection closed..." << std::endl;
    std::cout << "Goodbye..." << std::endl;
}

int TcpListener::_CreateSocket() const {
    int listening_fd = socket(AF_INET, SOCK_STREAM, 0);

    const int enable = 1;
    if (setsockopt(listening_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &enable, sizeof(int)) < 0)
        _handle_error("Error setting socket options.");

    if (listening_fd < 0)
        _handle_error("Error establishing connection.");

    std::cout << "Server Socket connection created..." << std::endl;

    struct sockaddr_in  server_addr;
    socklen_t           addr_size;

    server_addr.sin_family = AF_INET;   /* address family: AF_INET = ipv4 */
    server_addr.sin_port = htons(_port);    /* port in network byte order, htons() = host to network short */
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);    /* internet address -> address in network byte order, INADDR_ANY = any address */

    addr_size = sizeof(server_addr);

    if (bind(listening_fd, (struct sockaddr*)&server_addr, addr_size) < 0)
        _handle_error("Error binding socket.");

    if (listen(listening_fd, 1) < 0) {
        perror("Error while listening.");
        return -1;
    }
    else {
        //std::cout << blue << "| listen() is OK ! |" << def << std::endl;
        std::cout << "Looking for clients..." << std::endl;
    }
    return listening_fd;
}

int TcpListener::_WaitForConnection(int listening_fd) {
    struct sockaddr_in  client_addr;
    socklen_t           client_addr_size;
    int                 client_fd;

    client_addr_size = sizeof(client_addr);

    std::cout << "===DEBUG===" << "_WaitForConnection()" << std::endl;

    client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_fd < 0) {
        perror("Error on client connecting.");
        return -1;
    }

    std::cout << "new connection from: " << inet_ntoa(client_addr.sin_addr) << " on socket: " << client_fd << std::endl;

    std::cout << "Accepted connection !" << std::endl;

    std::string welcome = "Welcome !";
    MessageHandler::HandleMessage(client_fd, welcome);

    return client_fd;
}

void TcpListener::_handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
