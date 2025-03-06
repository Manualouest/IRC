/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:13:23 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/06 16:03:11 by mbirou           ###   ########.fr       */
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
	std::map<std::string, bool>	users;
	int							options[5];
	std::string					topic;
	std::string					password;
}								t_channelInfo;