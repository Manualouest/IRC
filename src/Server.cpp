/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:08:37 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/05 10:36:44 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::map<int, t_clientInfo*>	Server::_clients = std::map<int, t_clientInfo*>();
int								Server::_socketFd = 0;
int								Server::_pollFd = 0;
struct epoll_event				Server::_poll;
struct epoll_event				Server::_multPoll[MAX_EVENTS];
struct sockaddr_in				Server::_socket;
bool							Server::_running = false;

void	Server::_innit(const int &port)
{
	_socketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_socketFd == -1)
		_cleanstop(0, std::string("socket error: ") + std::string(strerror(errno)));
	_socket.sin_family = AF_INET;
	_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
	_socket.sin_port = htons(port);
	if (bind(_socketFd, (struct sockaddr *)&_socket, sizeof(_socket)) < 0)
		_cleanstop(0, std::string("bind error: ") + std::string(strerror(errno)));
	if(listen(_socketFd, 10) < 0)
		_cleanstop(0, std::string("listen error: ") + std::string(strerror(errno)));
	_pollFd = epoll_create1(0);
	if (_pollFd < 0)
		_cleanstop(0, std::string("epoll_create1 error: ") + std::string(strerror(errno)));
	_poll.events = EPOLLIN;
	_poll.data.fd = _pollFd;
	if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, _socketFd, &_poll) < 0)
		_cleanstop(0, std::string("epoll_ctl error: ") + std::string(strerror(errno)));
}

void	Server::ft_IRC(const int &port)
{
	int										clientFd;
	int										nbFds;
	socklen_t								socketSize;
	std::map<int, t_clientInfo*>::iterator	client;

	signal(SIGINT, Server::_shutdown);
	_innit(port);

	_running = !_running;
	while (_running)
	{
		nbFds = epoll_wait(_pollFd, _multPoll, MAX_EVENTS, 0);
		if (!_running)
			break ;
		if (nbFds < 0)
			_cleanstop(0, std::string("epoll_wait error: ") + std::string(strerror(errno)));
		for (int i = 0; i < nbFds; ++i)
		{
			if (_multPoll[i].data.fd == _pollFd)
			{
				clientFd = accept(_socketFd, (struct sockaddr *)&_socket, &(socketSize = sizeof(_socket)));
				if (clientFd < 0)
					_cleanstop(0, std::string("New client accept error: ") + std::string(strerror(errno)));
				_poll.events = EPOLLIN;
				_poll.data.fd = clientFd;
				if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, clientFd, &_poll) < 0)
					_cleanstop(clientFd, std::string("New client epoll_ctl error: ") + std::string(strerror(errno)));
				_clients.insert(_clients.begin(), std::pair<int, t_clientInfo*>(clientFd, _innitClient(clientFd)));
			}
			else
			{
				client = _clients.find(_multPoll[i].data.fd);
				if (_getCmd(client))
					_execCmd(client);
			}
		}
	}
	std::cout << "server stopping" << std::endl;
	_cleanstop(0, "");
}

bool	Server::_getCmd(std::map<int, t_clientInfo*>::iterator client)
{
	int		nbRecv;
	char	buffer[1024] = {0};

	do
	{
		memset(buffer, 0, sizeof(buffer));
		nbRecv = recv(client->first, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (nbRecv <= 0)
		{
			close(client->first);
			return (false);
		}
		client->second->cmd.append(buffer);
	} while (nbRecv == 1024 && buffer[nbRecv - 1] != 0 && buffer[nbRecv - 1] != '\n');
	return (buffer[nbRecv - 1] == '\n');
}

void	Server::_cleanstop(const int &extraFd, const std::string &error)
{
	for (std::map<int, t_clientInfo*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}
	if (extraFd > 0)
		close (extraFd);
	if (_socketFd > 0)
		close(_socketFd);
	if (_pollFd > 0)
		close(_pollFd);
	if (!error.empty())
		throw(Server::IrcFailException(error));
}

t_clientInfo	*Server::_innitClient(const int &clientFd)
{
	t_clientInfo	*client = new t_clientInfo;

	client->cmd = "";
	client->fd = clientFd;
	client->logged = false;
	client->nickname = "";
	client->realname = "";
	client->surname = "";
	return (client);
}

void	Server::_execCmd(std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "exec: " << client->second->cmd;
	client->second->cmd = "";
}

void	Server::_shutdown(int signal)
{
	if (signal == SIGINT)
	{
		if (_running)
			_running = false;
		else
		{
			_cleanstop(0, "");
			_running = true;
		}
	}
}

Server::IrcFailException::IrcFailException(const std::string &error)
{
	_error = error;
}

const char *Server::IrcFailException::what() const throw()
{
	if (!_error.empty())
		return (_error.c_str());
	return ("An error has occured.");
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (0);
	try
	{
		Server::ft_IRC(atoi(argv[1]));
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
