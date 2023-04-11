//
// Created by Samuel Bars on 3/22/23.
//

#ifndef FT_IRC_UTILS_HPP
#define FT_IRC_UTILS_HPP

#include <string>
#include <vector>

void		skip_line(std::string &msg);
bool		is_valid_nick(const std::string& nickname);
bool		is_irssi_client(const std::string &msg);
std::string	prep_message(std::string sender, std::string type, std::vector<std::string> &msg);

#endif //FT_IRC_UTILS_HPP
