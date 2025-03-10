/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/10 11:05:27 by mbirou           ###   ########.fr       */
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

typedef struct	s_clientInfo
{
	std::string	username;
	std::string	nickname;
	int			fd;
	bool		logged;
	bool		passed;
	std::string	cmd;
}				t_clientInfo;

typedef struct	s_channelInfo
{
	std::string					name;
	std::map<t_clientInfo*, bool>	users;
	int							options[5];
	std::string					topic;
	std::string					password;
}								t_channelInfo;