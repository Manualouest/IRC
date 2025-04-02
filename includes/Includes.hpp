/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/02 08:10:22 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <spellBook.hpp>
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

#define DOMAINNAME std::string("127.0.0.1")

// Err msg
#define ALRINCHAN(nick, arg, channel) std::string(":" + DOMAINNAME + " 443 " + (nick.empty() ? "*" : nick) + " " + arg + " #" + channel + " :Already in channel\r\n")
#define CHANNOTREAL(nick, channel) std::string(":" + DOMAINNAME + " 403 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Channel doesn't exist.\r\n")
#define NOTONCHAN(nick, channel) std::string(":" + DOMAINNAME + " 442 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Not on channel.\r\n")
#define WRONGPASS(nick) std::string(":" + DOMAINNAME + " 464 " + (nick.empty() ? "*" : nick) + " :Password is incorect\r\n")
#define CANTSENDTOCHAN(nick, channel) std::string(":" + DOMAINNAME + " 404 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Cannot send to channel.\r\n")
#define NOSUCHNICK(nick, arg) std::string(":" + DOMAINNAME + " 401 " + (nick.empty() ? "*" : nick) + " " + arg + " :This nickname doesn't exist.\r\n")
#define NOSUCHNICKCHAN(nick, arg) std::string(":" + DOMAINNAME + " 401 " + (nick.empty() ? "*" : nick) + " " + arg + " :No such nick/channel.\r\n")
#define NOTENOUGHARGS(nick) std::string(":" + DOMAINNAME + " 461 " + (nick.empty() ? "*" : nick) + " :Not enough arguments given\r\n")
#define NOPASSGIVEN(nick) std::string(":" + DOMAINNAME + " 464 " + (nick.empty() ? "*" : nick) + " :Password was not given\r\n")
#define NONICKGIVEN(nick) std::string(":" + DOMAINNAME + " 431 " + (nick.empty() ? "*" : nick) + " :Please enter a nickname\r\n")
#define UNKNOWN(nick, error) std::string(":" + DOMAINNAME + " 421 " + (nick.empty() ? "*" : nick) + " :" + error + "\r\n")
#define BADCHANNAME(nick, arg) std::string(":" + DOMAINNAME + " 479 " + (nick.empty() ? "*" : nick) + " " + arg +" :Illegal channel name\r\n")
#define	NOTOPERATOR(nick, channel) std::string(":" + DOMAINNAME + " 482 " + (nick.empty() ? "*" : nick) + " #" + channel + " :You're not channel operator.\r\n")
#define ERRMODEL(nick, channel) std::string(":" + DOMAINNAME + " 471 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Cannot join channel (+l)\r\n")
#define ERRMODEK(nick, channel) std::string(":" + DOMAINNAME + " 475 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Cannot join channel (+k)\r\n")
#define ERRMODEI(nick, channel) std::string(":" + DOMAINNAME + " 473 " + (nick.empty() ? "*" : nick) + " #" + channel + " :Cannot join channel (+i)\r\n")
#define ERRMODEUNKNOW(nick, arg) std::string(":" + DOMAINNAME + " 472 " + (nick.empty() ? "*" : nick) + " " + arg + " :is unknown mode char to me\r\n")
#define USERNOTINCHANNEL(nick, arg) std::string(":" + DOMAINNAME + " 441 " + (nick.empty() ? "*" : nick) + " " + arg + " :They aren't on that channel\r\n")

// Cmd replyUNKNOWN
#define USERJOIN(nick, channel) std::string(":" + (nick.empty() ? "*" : nick) + " JOIN #" + channel + "\r\n")
#define JOINTOPIC(nick, channel, topic) std::string(":" + DOMAINNAME + " 332 " + (nick.empty() ? "*" : nick) + " #" + channel + " :" + topic + "\r\n")
#define USERPART(nick, channel, reason) std::string(":" + (nick.empty() ? "*" : nick) + " PART " + channel + " " + reason + "\r\n")
#define PRIVMSG(nick, target, msg) std::string(":" + (nick.empty() ? "*" : nick) + " PRIVMSG " + target + " " + msg + "\r\n")
#define QUITMSG(nick, reason) std::string(":" + (nick.empty() ? "*" : nick) + " QUIT :Quit: " + reason.substr((client->second->cmdtoken.args[0].find(':') != std::string::npos ? client->second->cmdtoken.args[0].find(':') : 0)) + "\r\n")
#define WELCOMEMSG(nick) std::string(":" + DOMAINNAME + " 001 " + (nick.empty() ? "*" : nick) + " :Welcome to ft_IRC\r\n")
#define NAMESTART(nick, channel) std::string(":" + DOMAINNAME + " 353 " + (nick.empty() ? "*" : nick) + " = #" + channel + " :")
#define NAMELIST(client) std::string((client->second ? " @" : " ") + client->first->nickname)
#define NAMEEND(nick, channel) std::string("\r\n:" + DOMAINNAME + " 366 " + (nick.empty() ? "*" : nick) + " #" + channel + " :End of /NAMES list\r\n")
#define HASKICKED(nick, channel, user, arg) std::string((nick.empty() ? "*" : nick) + " has kicked " + user + " from #" + channel + " (" + arg + ")\r\n")
#define BEENKICKED(nick, channel, arg) std::string("You have been kicked from #" + channel + " by " + nick + " (" + arg + ")\r\n")
#define USERKICK(nick, channel, user, arg) std::string(":" + (nick.empty() ? "*" : nick) + " KICK #" + channel + " " + user + " :" + arg + "\r\n")

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
	t_cmdtoken	usertoken;
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
