/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 14:30:53 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <csignal>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <vector>
#include <set>
#include <sstream>

// Err msg
#define ALRINCHAN(nick) std::string(":127.0.0.1 " + nick + " :Already in channel\r\n")
#define CHANNOTREAL(nick) std::string(":127.0.0.1 403 " + nick + " :Channel doesn't exist.\r\n")
#define NOTONCHAN(nick) std::string(":127.0.0.1 442 " + nick + " :Not on channel.\r\n")
#define WRONGPASS(nick) std::string(":127.0.0.1 464 " + nick + " :Password is incorect\r\n")
#define CANTSENDTOCHAN(nick) std::string(":127.0.0.1 404 " + nick + " :Cannot send to channel.\r\n")
#define NOSUCHNICK(nick) std::string(":127.0.0.1 401 " + nick + ":This nickname doesn't exist.\r\n")
#define NOTENOUGHARGS(nick) std::string(":127.0.0.1 461 " + nick + " :Not enough arguments given\r\n")
#define NOPASSGIVEN(nick) std::string(":127.0.0.1 464 " + nick + " :Password was not given\r\n")
#define NONICKGIVEN(nick) std::string(":127.0.0.1 431 " + nick + " :Please enter a nickname\r\n")
#define UNKNOWN(nick, error) std::string(":127.0.0.1 421 " + nick + " :" + error + "\r\n")
#define BADCHANNAME(nick) std::string(":127.0.0.1 479 " + nick + " :Illegal channel name\r\n")

// Cmd replyUNKNOWN
#define USERJOIN(nick, channel) std::string(":" + nick + " JOIN #" + channel + "\r\n")
#define USERPART(nick, channel, reason) std::string(":" + nick + " PART " + channel + " " + reason + "\r\n")
#define PRIVMSG(nick, target, msg) std::string(":" + nick + " PRIVMSG " + target + " " + msg + "\r\n")
#define QUITMSG(nick, reason) std::string(":" + nick + " QUIT :Quit: " + reason.substr((client->second->cmdtoken.args[0].find(':') != std::string::npos ? client->second->cmdtoken.args[0].find(':') : 0)) + "\r\n")
#define WELCOMEMSG(nick) std::string(":127.0.0.1 001 " + nick + " :Welcome to ft_IRC\r\n")
#define NAMESTART(nick, channel) std::string(":127.0.0.1 353 " + nick + " = #" + channel + " :")
#define NAMELIST(client) std::string((client->second ? " @" : " ") + client->first->nickname)
#define NAMEEND(nick, channel) std::string("\r\n:127.0.0.1 366 " + nick + " #" + channel + " :End of /NAMES list\r\n")

typedef struct s_cmdtoken
{
	std::string					cmd;
	std::string					target;
	std::vector<std::string>	args;
}								t_cmdtoken;

typedef struct	s_clientInfo
{
	std::string	username;
	std::string	nickname;
	std::string	realname;
	int			fd;
	bool		logged;
	bool		passed;
	std::string	cmd;
	t_cmdtoken	cmdtoken;
}				t_clientInfo;

typedef struct	s_channelInfo
{
	std::string						name;
	std::map<t_clientInfo*, bool>	users;
	int								options[5];
	std::string						topic;
	std::string						password;
}								t_channelInfo;

class Utils
{
	public:
		static void Send(const int &fd, const std::string &msg);

	private:
		Utils();
		~Utils();
};
