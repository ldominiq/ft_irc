
#include "TcpListener.hpp"

int main() {
    //    if (argc != 3) {
    //        std::cout << "./ircserv <port> <password>" << std::endl;
    //        return 1;
    //    }

    TcpListener server("127.0.0.1", 6667);
	populateCommands();
    server.Run();

    return 0;
}