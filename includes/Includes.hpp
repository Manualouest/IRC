/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/31 16:44:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <csignal>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>

// Err msg
#define ALRINCHAN(nick, arg, channel) std::string(":127.0.0.1 443 " + nick + " " + arg + " #" + channel + " :Already in channel\r\n")
#define CHANNOTREAL(nick, channel) std::string(":127.0.0.1 403 " + nick + " #" + channel + " :Channel doesn't exist.\r\n")
#define NOTONCHAN(nick, channel) std::string(":127.0.0.1 442 " + nick + " #" + channel + " :Not on channel.\r\n")
#define WRONGPASS(nick) std::string(":127.0.0.1 464 " + nick + " :Password is incorect\r\n")
#define CANTSENDTOCHAN(nick, channel) std::string(":127.0.0.1 404 " + nick + " #" + channel + " :Cannot send to channel.\r\n")
#define NOSUCHNICK(nick, arg) std::string(":127.0.0.1 401 " + nick + " " + arg + " :This nickname doesn't exist.\r\n")
#define NOSUCHNICKCHAN(nick, arg) std::string(":127.0.0.1 401 " + nick + " " + arg + " :No such nick/channel.\r\n")
#define NOTENOUGHARGS(nick) std::string(":127.0.0.1 461 " + nick + " :Not enough arguments given\r\n")
#define NOPASSGIVEN(nick) std::string(":127.0.0.1 464 " + nick + " :Password was not given\r\n")
#define NONICKGIVEN(nick) std::string(":127.0.0.1 431 " + nick + " :Please enter a nickname\r\n")
#define UNKNOWN(nick, error) std::string(":127.0.0.1 421 " + nick + " :" + error + "\r\n")
#define BADCHANNAME(nick, arg) std::string(":127.0.0.1 479 " + nick + " " + arg +" :Illegal channel name\r\n")
#define	NOTOPERATOR(nick, channel) std::string(":127.0.0.1 482 " + nick + " #" + channel + " :You're not channel operator.\r\n")
#define ERRMODEL(nick, channel) std::string(":127.0.0.1 471 " + nick + " #" + channel + " :Cannot join channel (+l)\r\n")
#define ERRMODEK(nick, channel) std::string(":127.0.0.1 475 " + nick + " #" + channel + " :Cannot join channel (+k)\r\n")
#define ERRMODEI(nick, channel) std::string(":127.0.0.1 473 " + nick + " #" + channel + " :Cannot join channel (+i)\r\n")
#define ERRMODEUNKNOW(nick, arg) std::string(":127.0.0.1 472 " + nick + " " + arg + " :is unknown mode char to me\r\n")
#define USERNOTINCHANNEL(nick, arg) std::string(":127.0.0.1 441 " + nick + " " + arg + " :They aren't on that channel\r\n")

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
	bool							isTopicOPOnly;
	bool							isInviteOnly;
	int								limit;
	std::set<std::string>			invite;
}								t_channelInfo;

class Utils
{
	public:
		static void Send(const int &fd, const std::string &msg);

	private:
		Utils();
		~Utils();
};
