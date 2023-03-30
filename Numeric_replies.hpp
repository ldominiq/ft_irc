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

#endif //FT_IRC_NUMERIC_REPLIES_HPP
