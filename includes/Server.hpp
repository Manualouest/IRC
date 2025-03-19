/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:24:52 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/17 11:29:26 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Includes.hpp>
#include <Commands.hpp>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024
#define MAX_CLIENT 10
#define CMDSNAME (std::string[8]){"PASS ", "NICK ", "USER ",  "JOIN ", "PRIVMSG ", "PART ", "QUIT ", "INVITE "}

class Server
{
	public:
		static void	ft_IRC(const int &port, const std::string &pass);
		static int	_findUser(const std::string &name);

	private:
		Server();
		~Server();

		static void			_init(const int &port);
		static t_clientInfo	*_initClient(const int &clientFd);
		static bool			_getCmd(std::map<int, t_clientInfo*>::iterator client);
		static void			_execCmd(std::map<int, t_clientInfo*>::iterator client);
		static void			_cleanstop(const int &extraFd, const std::string &error);
		static void			_shutdown(int signal);
		static void			_delUser(t_clientInfo *user);
		
		static std::map<int, t_clientInfo*>				_clients;
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

		class Channels : public ChannelCommands
		{};
};
