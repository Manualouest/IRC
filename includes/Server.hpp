/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:24:52 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/05 13:51:26 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <csignal>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define MAX_CLIENT 10

typedef struct	s_clientInfo
{
	std::string	realname;
	std::string	surname;
	std::string	nickname;
	int			fd;
	bool		logged;
	std::string	cmd;
}				t_clientInfo;

class Server
{
	public:
		static void	ft_IRC(const int &port);

	private:
		Server();
		Server(const Server &src);
		Server	operator =(const Server &rhs);
		~Server();

		static void			_init(const int &port);
		static t_clientInfo	*_initClient(const int &clientFd);
		static bool			_getCmd(std::map<int, t_clientInfo*>::iterator client);
		static void			_execCmd(std::map<int, t_clientInfo*>::iterator client);
		static void			_cleanstop(const int &extraFd, const std::string &error);
		static void			_shutdown(int signal);

		static std::map<int, t_clientInfo*>	_clients;
		static int							_socketFd;
		static int							_pollFd;
		static struct epoll_event			_poll;
		static struct epoll_event			_multPoll[MAX_EVENTS];
		static struct sockaddr_in			_socket;
		static bool							_running;

		class IrcFailException : public std::exception
		{
			public:
				IrcFailException(const std::string &error);
				virtual ~IrcFailException() throw() {}
				virtual const char *what() const throw();

			private:
				std::string	_error;
		};
};
