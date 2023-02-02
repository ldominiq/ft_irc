#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 200

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

void    handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier red(Color::FG_RED);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier def(Color::FG_DEFAULT);

//    if (argc != 3) {
//        std::cout << "./ircserv <port> <password>" << std::endl;
//        return 1;
//    }

    // 1. Create socket - socket()
    int listening_fd;

    listening_fd = socket(AF_INET, SOCK_STREAM, 0);

    const int enable = 1;
    if (setsockopt(listening_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &enable, sizeof(int)) < 0)
        handle_error("Error setting socket options.");

    if (listening_fd < 0)
        handle_error("Error establishing connection.");

    //std::cout << green << "| socket() is OK ! |" << def << std::endl;
    std::cout << green << "Server Socket connection created..." << def << std::endl;

    // 2. Bind the socket to an ip address and port - bind()
    struct sockaddr_in  server_addr;
    socklen_t           addr_size;
    int                 port = 6667;

    server_addr.sin_family = AF_INET;   /* address family: AF_INET = ipv4 */
    server_addr.sin_port = htons(port);    /* port in network byte order, htons() = host to network short */
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);    /* internet address -> address in network byte order, INADDR_ANY = any address */

    addr_size = sizeof(server_addr);

    if (bind(listening_fd, (struct sockaddr*)&server_addr, addr_size) < 0)
        handle_error("Error binding socket.");

    //std::cout << blue << "| bind() is OK ! |" << def << std::endl;

    // 3. Listen on the socket - listen()
    if (listen(listening_fd, 1) < 0)
        perror("Error while listening.");
    else {
        //std::cout << blue << "| listen() is OK ! |" << def << std::endl;
        std::cout << blue << "Looking for clients..." << def << std::endl;
    }

    // 4. Wait and Accept a connection - accept(), connect()
    struct sockaddr_in  client_addr;
    socklen_t           client_addr_size;
    int                 client_fd;

    client_addr_size = sizeof(client_addr);

    client_fd = accept(listening_fd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_fd < 0)
        handle_error("Error on accepting.");

//    char host[NI_MAXHOST];      // Client's remote name
//    char service[NI_MAXSERV];    // Service (i.e. port) the client is connected on
//
//    memset(host, 0, NI_MAXHOST);
//    memset(service, 0, NI_MAXSERV);
//
//    struct addrinfo *result;
//
//    int s = getaddrinfo(host,
//                    service,
//                    (addrinfo *)&client_fd,
//                    &result);
//
    std::cout << inet_ntoa(client_addr.sin_addr) << std::endl;

//    freeaddrinfo(result);

    std::cout << green << "Accepted connection !" << def << std::endl;

    // 5. Send and receive data - recv(), send()
    char buffer[BUF_SIZE];

    while (true) {
        memset(buffer, 0, BUF_SIZE);

        // Wait for client to send data
        ssize_t bytes_received = recv(client_fd, buffer, BUF_SIZE, 0);
        if (bytes_received == 0) {
            std::cout << blue << "Client disconnected" << std::endl;
            break;
        }

        if (bytes_received > 0)
            std::cout << "Message received: " << buffer << std::endl;

        // Echo message back to client

        char confirmation[BUF_SIZE] = "Message received\n";
        bytes_received = send(client_fd, confirmation, BUF_SIZE, 0);

        if (bytes_received > 0)
            std::cout << "Automated Message sent to the Client" << std::endl;

    }

    // 6. Disconnect - close()
    close(listening_fd);
    std::cout << red << "Server Socket connection closed..." << def << std::endl;
    std::cout << "Goodbye..." << std::endl;
    return 0;
}
