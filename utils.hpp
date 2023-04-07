//
// Created by Samuel Bars on 3/22/23.
//

#ifndef FT_IRC_UTILS_HPP
#define FT_IRC_UTILS_HPP

#include <string>

void	_skip_line(std::string &msg);
bool	is_valid_nick(const std::string& nickname);
bool	is_irssi_client(const std::string &msg);

#endif //FT_IRC_UTILS_HPP
