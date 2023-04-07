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

#include <regex>

bool is_valid_nick(const std::string& nickname) {
    // The nickname must start with a letter (a-z or A-Z) or one of the allowed special characters (| or [).
    // The nickname can be followed by any combination of letters, digits, and allowed special characters.
    // The nickname must be between 1 and 9 characters in length.
	// todo: use cpp98 function
	if (!std::regex_match(nickname, std::regex("^[a-zA-Z|[]([a-zA-Z0-9|\\[\\]\\`_^{}\\-]{0,8})?$"))) {
        return false;
    }
    return true;
}

bool	is_irssi_client(const std::string &msg) {
	if (msg.find("CAP LS") != std::string::npos &&
		msg.find("NICK") != std::string::npos &&
		msg.find("USER") != std::string::npos )
	{
		return true;
	}
	return false;
}

std::string	prep_message(std::string sender, std::string type, std::vector<std::string> &msg)
{
	std::string message = ":" + sender + " " + type + " ";
	for (std::vector<std::string>::iterator it = msg.begin(); it != msg.end(); it++)
		message += *it + " ";
	return message;
}
