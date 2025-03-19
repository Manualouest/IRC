/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:08:37 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/17 09:52:14 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
https://www.alien.net.au/irc/irc2numerics.html
https://modern.ircdocs.horse/#client-to-server-protocol-structure
*/


#include "../includes/Server.hpp"

std::map<int, t_clientInfo*>			Server::_clients = std::map<int, t_clientInfo*>();
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

	std::cout << "Server is starting" << std::endl;
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
	for (std::map<std::string, t_channelInfo*>::const_iterator it = Channels::begin(); it != Channels::end(); ++it)
		delete it->second;
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
	client->realname = "";
	return (client);
}

void	filltoken(t_cmdtoken &token, const std::string &cmd)
{
	token.cmd = cmd.substr(0, cmd.find_first_of(' ') + 1);
	token.target = "";
	if (!strcmp(token.cmd.c_str(), "JOIN ") || !strcmp(token.cmd.c_str(), "MODE ") || !strcmp(token.cmd.c_str(), "KICK ") || !strcmp(token.cmd.c_str(), "PRIVMSG ")
		 || !strcmp(token.cmd.c_str(), "PART "))
		token.target = cmd.substr(token.cmd.length(), cmd.find_first_of(" \r\n", token.cmd.length()) - token.cmd.length());
	token.args.clear();
	std::string			lineArg;
	std::stringstream	rawString(&cmd.c_str()[token.cmd.length() + token.target.length() + !token.target.empty()]);
	std::cout << "cmd: " << token.cmd << ": ";
	while (std::getline(rawString, lineArg, ' '))
	{
		if (lineArg[0] == ':')
			lineArg.append(cmd.substr(cmd.find(lineArg) + lineArg.length(), cmd.length()));
		std::cout << "arg: " << lineArg << "; ";
		lineArg = lineArg.substr(0, std::min(lineArg.length(), lineArg.find_first_of("\r\n\0") - 1));
		token.args.push_back(lineArg);
		if (lineArg[0] == ':')
			break ;
	}
	std::cout << std::endl;
}

void	Server::_execCmd(std::map<int, t_clientInfo*>::iterator client)
{
	while (!client->second->cmd.empty())
	{
		// std::cout << "in: " << client->second->cmd.find_first_of("\r\n") << ", " << client->second->cmd.length() - 2 << ", " << client->second->cmd;
		std::string	cmd = client->second->cmd.substr(0, client->second->cmd.find_first_of("\r\n"));
		filltoken(client->second->cmdtoken, cmd);
		std::cout << "exec: '''" << cmd << std::endl << "'''" << std::endl;
		int	func = 0;
		for (; func < 8; ++func)
		{
			if (CMDSNAME[func] == client->second->cmdtoken.cmd)
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
				Commands::join(client);
				break;
			case 4:
				Commands::privmsg(client, _findUser);
				break;
			case 5:
				Commands::part(client);
				break;
			case 6:
				Commands::quit(client, _delUser);
				return;
			case 7:
				Commands::invite(client);
				break;
			default:
				std::cout << "erm idk what that is" << std::endl;
		}
		if (client->second->cmd.find_first_of("\r\n") == client->second->cmd.length() - 2)
			client->second->cmd = "";
		else
			client->second->cmd = client->second->cmd.substr(client->second->cmd.find_first_of("\r\n") + 2);
	}
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

int	Server::_findUser(const std::string &name)
{
	for (std::map<int, t_clientInfo*>::iterator	client = _clients.begin(); client != _clients.end(); ++client)
		if (client->second->nickname == name)
			return (client->first);
	return (-1);
}

void	Server::_delUser(t_clientInfo *user)
{
	_clients.erase(user->fd);
	close(user->fd);
	delete user;
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
