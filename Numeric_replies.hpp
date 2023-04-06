//
// Created by Lucas Dominique on 3/28/23.
//

#ifndef FT_IRC_NUMERIC_REPLIES_HPP
#define FT_IRC_NUMERIC_REPLIES_HPP

#define user_id(nickname, username) (nickname + "!" + username + "@localhost")
#define RPL_WELCOME(nick) (":localhost 001 " + nick + " :Welcome to the COOL IRC Network, " + user_id + "\r\n")
#define RPL_YOURHOST(nick, version) (":localhost 002 " + nick + " :Your host is localhost, running version " + version + "\r\n")
#define RPL_CREATED(nick, datetime) (":localhost 003 " + nick + " :This server was created " + datetime + "\r\n")
#define RPL_MYINFO(nick, version, user_modes, channel_modes) (":localhost 004 " + nick + " gigacoolchat " + version + " " + user_modes + " " + channel_modes + "\r\n")

#define RPL_MOTDSTART(nick) (":localhost 375 " + nick + " :- localhost Message of the day - \r\n")
#define RPL_MOTD(nick, motd_line) (":localhost 372 " + nick + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(nick) (":localhost 376 " + nick + " :End of /MOTD command.\r\n")

#define RPL_TOPIC(nick, channel, topic) (":localhost 332 " + nick + " " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(nick, channel, users) (":localhost 353 " + nick + " = " + channel + " :" + users + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (":localhost 366 " + nick + " " + channel + " :End of /NAMES list" + "\r\n")

#define RPL_YOUREOPER(nick) (":localhost 381 " + nick + " :You are now an IRC operator\r\n")

// ERRORS
#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTERED(client) (":localhost 462 " + client + " :You are already registered\r\n")
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")
#define ERR_NOOPERHOST(client) (":localhost 491 " + client + " :Invalid credentials\r\n")
#define ERR_NOSUCHNICK(user_id, target) (":localhost 401 " + user_id + " " + target + " :No such nick\r\n")
#define ERR_NOSUCHCHANNEL(user_id, target) (":localhost 403 " + user_id + " " + target + " :No such channel\r\n")
#define ERR_USERSDONTMATCH(user_id) (":localhost 502 " + user_id + " :Cant change mode for other users\r\n")
#define ERR_NOPRIVILEGES(user_id) (":localhost 481 " + user_id + " :Permission Denied- You're not an IRC operator\r\n")

#endif //FT_IRC_NUMERIC_REPLIES_HPP
