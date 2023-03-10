//
// Created by Lucas on 24-Feb-23.
//

#include "Client.hpp"

Client::Client(int fd) : _clientFd(fd), _nickname(), _username(), _channels(){

}

Client::~Client() {

}
