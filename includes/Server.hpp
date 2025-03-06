/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:24:52 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/06 15:51:18 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Includes.hpp>
#include <Commands.hpp>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define MAX_CLIENT 10
#define CMDSNAME (std::string[5]){"PASS ", "NICK ", "USER ", "QUIT ",  "JOIN "}

class Server
{
	public:
		static void	ft_IRC(const int &port, const std::string &pass);

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

		static std::map<int, t_clientInfo*>				_clients;
		static std::map<std::string, t_channelInfo*>	_channels;
		static int										_socketFd;
		static int										_pollFd;
		static struct epoll_event						_poll;
		static struct epoll_event						_multPoll[MAX_EVENTS];
		static struct sockaddr_in						_socket;
		static bool										_running;
		static std::string								_pass;

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
