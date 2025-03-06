/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:08:37 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/06 16:07:34 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

std::map<int, t_clientInfo*>			Server::_clients = std::map<int, t_clientInfo*>();
std::map<std::string, t_channelInfo*>	Server::_channels = std::map<std::string, t_channelInfo*>();
int										Server::_socketFd = 0;
int										Server::_pollFd = 0;
struct epoll_event						Server::_poll;
struct epoll_event						Server::_multPoll[MAX_EVENTS];
struct sockaddr_in						Server::_socket;
bool									Server::_running = false;
std::string								Server::_pass = "";

void	Server::_init(const int &port)
{
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
		_cleanstop(0, std::string("socket error: ") + std::string(strerror(errno)));
	_socket.sin_family = AF_INET;
	_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
	_socket.sin_port = htons(port);
	if (bind(_socketFd, (struct sockaddr *)&_socket, sizeof(_socket)) < 0)
		_cleanstop(0, std::string("bind error: ") + std::string(strerror(errno)));
	if(listen(_socketFd, MAX_CLIENT) < 0)
		_cleanstop(0, std::string("listen error: ") + std::string(strerror(errno)));
	_pollFd = epoll_create1(0);
	if (_pollFd < 0)
		_cleanstop(0, std::string("epoll_create1 error: ") + std::string(strerror(errno)));
	_poll.events = EPOLLIN;
	_poll.data.fd = _pollFd;
	if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, _socketFd, &_poll) < 0)
		_cleanstop(0, std::string("epoll_ctl error: ") + std::string(strerror(errno)));
}

void	Server::ft_IRC(const int &port, const std::string &pass)
{
	int										clientFd;
	int										nbFds;
	socklen_t								socketSize;
	std::map<int, t_clientInfo*>::iterator	client;

	signal(SIGINT, Server::_shutdown);
	_pass = pass;
	_init(port);

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
				_poll.events = EPOLLIN | EPOLLET;
				_poll.data.fd = clientFd;
				_clients.insert(_clients.begin(), std::pair<int, t_clientInfo*>(clientFd, _initClient(clientFd)));
				if (epoll_ctl(_pollFd, EPOLL_CTL_ADD, clientFd, &_poll) < 0)
					_cleanstop(0, std::string("New client epoll_ctl error: ") + std::string(strerror(errno)));
			}
			else
			{
				client = _clients.find(_multPoll[i].data.fd);
				if (_getCmd(client))
					_execCmd(client);
			}
		}
	}
	std::cout << "\33[2K\rserver is stopping" << std::endl;
	_cleanstop(0, "");
}

bool	Server::_getCmd(std::map<int, t_clientInfo*>::iterator client)
{
	int		nbRecv;
	char	buffer[BUFFER_SIZE] = {0};

	do
	{
		memset(buffer, 0, sizeof(buffer));
		nbRecv = recv(client->first, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (nbRecv <= 0 || (nbRecv > 1 && buffer[nbRecv - 1] == '\n' && buffer[nbRecv - 2] != '\r'))
		{
			delete client->second;
			close(client->first);
			_clients.erase(client->first);
			return (false);
		}
		client->second->cmd.append(buffer);
	} while (nbRecv == BUFFER_SIZE && buffer[nbRecv - 1] != 0 && buffer[nbRecv - 2] != '\r' && buffer[nbRecv - 1] != '\n');
	return (nbRecv > 1 && buffer[nbRecv - 2] == '\r' && buffer[nbRecv - 1] == '\n');
}

void	Server::_cleanstop(const int &extraFd, const std::string &error)
{
	for (std::map<int, t_clientInfo*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete it->second;
		close(it->first);
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

t_clientInfo	*Server::_initClient(const int &clientFd)
{
	t_clientInfo	*client = new t_clientInfo;

	client->cmd = "";
	client->fd = clientFd;
	client->logged = false;
	client->passed = false;
	client->nickname = "";
	client->username = "";
	return (client);
}

void	Server::_execCmd(std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "exec: " << client->second->cmd;
	int	func = 0;
	for (; func < 5; ++func)
	{
		if (CMDSNAME[func] == client->second->cmd.substr(0, client->second->cmd.find_first_of(' ') + 1))
			break ;
	}
	switch (func)
	{
		case 0:
			Commands::pass(client, _pass);
			break ;
		case 1:
			Commands::nick(client);
			break;
		case 2:
			Commands::user(client);
			break;
		case 3:

			break;
		case 4:

			break;
		default:
			std::cout << "erm idk what that is" << std::endl;
	}
	client->second->cmd = "";
	send(client->first, "", 0, MSG_DONTWAIT);
}

/*
PASS 1234
NICK test
USER test 0 * :test
QUIT :Leaving*/

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
	if (argc != 3)
		return (0);
	try
	{
		Server::ft_IRC(atoi(argv[1]), argv[2]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
