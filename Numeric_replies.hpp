//
// Created by Lucas Dominique on 3/28/23.
//

#ifndef FT_IRC_NUMERIC_REPLIES_HPP
#define FT_IRC_NUMERIC_REPLIES_HPP

#define user_id(nickname, username) (nickname + "!" + username + "@localhost")
#define RPL_WELCOME(user_id, nick) (":localhost 001 " + nick + " :Welcome to the COOL IRC Network, " + user_id + "\r\n")
#define RPL_YOURHOST(nick) (":localhost 002 " + nick + " :Your host is localhost, running version DEV.1\r\n")
#define RPL_CREATED(datetime) (":localhost 003 " + nick + " :This server was created " + datetime + "\r\n")
#define RPL_MYINFO() (":localhost 004 gigacoolchat 0.0.1 +i +i\r\n")

#define RPL_MOTDSTART(client) (":localhost 375 " + client + " :- localhost Message of the day - \r\n")
#define RPL_MOTD(client, motd_line) (":localhost 372 " + client + " :" + motd_line + "\r\n")
#define RPL_ENDOFMOTD(client) (":localhost 376 " + client + " :End of /MOTD command.\r\n")

#define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(client, channel, users) (":localhost 353 " + client + " = " + channel + " :" + users + "\r\n")
#define RPL_ENDOFNAMES(client, channel) (":localhost 366 " + client + " " + channel + " :End of /NAMES list" + "\r\n")

#define RPL_YOUREOPER(client) (":localhost 381 " + client + " :You are now an IRC operator\r\n")

// ERRORS

#define ERR_NEEDMOREPARAMS(client, command) (":localhost 461 " + client + " " + command + " :Not enough parameters\r\n")
#define ERR_PASSWDMISMATCH(client) (":localhost 464 " + client + " :Password incorrect\r\n")
#define ERR_NOOPERHOST(client) (":localhost 491 " + client + " :Invalid credentials\r\n")

#endif //FT_IRC_NUMERIC_REPLIES_HPP
