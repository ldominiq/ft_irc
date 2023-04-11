//
// Created by Samuel Bars on 3/22/23.
//

#include "utils.hpp"

void skip_line(std::string &msg)
{
	size_t pos = msg.find_first_of("\r\n");
	if (pos != std::string::npos) {
		msg.erase(0, pos + 2);
	} else {
		msg.clear();
	}
}

bool is_valid_nick(const std::string& nickname) {
	const std::string valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ|[]0123456789`_^{}-";
	const std::string allowed_special_chars = "|[";

	// The nickname must be between 1 and 9 characters in length.
	if (nickname.length() < 1 || nickname.length() > 9) {
		return false;
	}

	// The nickname must start with a letter (a-z or A-Z) or one of the allowed special characters (| or [).
	if (valid_chars.find_first_of(nickname[0]) == std::string::npos &&
		allowed_special_chars.find_first_of(nickname[0]) == std::string::npos) {
		return false;
	}

	// The nickname can be followed by any combination of letters, digits, and allowed special characters.
	const std::string nickname_chars = nickname.substr(1);
	if (nickname_chars.find_first_not_of(valid_chars) != std::string::npos) {
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
