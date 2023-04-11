
#include "TcpListener.hpp"

int main(int argc, char **argv) {
        if (argc != 3) {
            std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
            return 1;
        }

    TcpListener server("127.0.0.1", atoi(argv[1]), argv[2]);
    server.Run();

    return 0;
}
