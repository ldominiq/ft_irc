#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ostream>
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
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        handle_error("Error establishing connection.");

    std::cout << green << "| socket() is OK ! |" << def << std::endl;
    std::cout << green << "Server Socket connection created..." << def << std::endl;

    // 2. Bind the socket = bind()
    struct sockaddr_in  server_addr;
    socklen_t           addr_size;
    int                 port = 6667;

    server_addr.sin_family = AF_INET;   /* address family: AF_INET */
    server_addr.sin_port = htons(port);    /* port in network byte order */
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);    /* internet address -> address in network byte order */

    addr_size = sizeof(server_addr);

    if (bind(sockfd, (struct sockaddr*)&server_addr, addr_size) < 0)
        handle_error("Error binding socket.");

    std::cout << blue << "| bind() is OK ! |" << def << std::endl;

    // 3. Listen on the socket - listen()
    if (listen(sockfd, 1) < 0)
        perror("Error while listening.");
    else {
        std::cout << blue << "| listen() is OK ! |" << def << std::endl;
        std::cout << blue << "Looking for clients..." << def << std::endl;
    }

    // 4. Accept a connection - accept(), connect()
    int servfd;

    servfd = accept(sockfd, (struct sockaddr*)&server_addr, &addr_size);
    if (servfd < 0)
        handle_error("Error on accepting.");

    std::cout << green << "Accepted connection !" << def << std::endl;

    // 5. Send and receive data - recv(), send(), recvfrom(), sendto()


    // 6. Disconnect - closesocket()
    close(sockfd);
    std::cout << red << "Server Socket connection closed..." << def << std::endl;
    std::cout << "Goodbye..." << std::endl;
    return 0;
}
