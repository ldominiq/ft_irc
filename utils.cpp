//
// Created by Samuel Bars on 3/22/23.
//

#include "utils.hpp"

void _skip_line(std::string &msg)
{
	size_t pos = msg.find_first_of("\r\n");
	if (pos != std::string::npos) {
		msg.erase(0, pos + 2);
	} else {
		msg.clear();
	}
}

void numericReply(int client_fd, int numeric, const std::string &message)
{
	std::string reply = ":127.0.0.1 " + std::to_string(numeric) + " " + message;
	TcpListener::Send(client_fd, reply);
}

#include <regex>

bool is_valid_nick(const std::string& nickname) {
	// The nickname must start with a letter (a-z or A-Z) or one of the allowed special characters (| or [).
	if (!std::regex_match(nickname, std::regex("^[a-zA-Z|\\[]"))) {
		return false;
	}
	// The nickname can be followed by any combination of letters, digits, and allowed special characters.
	if (!std::regex_match(nickname, std::regex("^[a-zA-Z0-9|\\[\\]\\\\`_^{}\\-]*$"))) {
		return false;
	}
	// The nickname must be between 1 and 9 characters long.
	if (!(1 <= nickname.length() && nickname.length() <= 9)) {
		return false;
	}

	// The nickname is valid.
	return true;
}