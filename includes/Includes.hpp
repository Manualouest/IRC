/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/17 13:26:02 by mbatty           ###   ########.fr       */
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
#include <sstream>
#include <utility>

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
	int								limit;
	bool							isTopicOPOnly;
	bool							isInviteOnly;
}								t_channelInfo;

class Utils
{
	public:
		static void Send(const int &fd, const std::string &msg);

	private:
		Utils();
		~Utils();
};
