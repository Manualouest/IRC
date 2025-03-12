/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:11:44 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/12 10:41:33 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

bool Commands::checks(std::map<int, t_clientInfo*>::iterator client, int choice)
{
	if (choice & NOARGS && !client->second->cmd.length())
	{
		Utils::Send(client->first, ":127.0.0.1 421 * :No argument given\r\n");
		return (false);
	}
	if (choice & NOPASS && !client->second->passed)
	{
		Utils::Send(client->first, ":127.0.0.1 464 * :Password was not given\r\n");
		return (false);
	}
	if (choice & NONICK && client->second->nickname.empty())
	{
		Utils::Send(client->first, ":127.0.0.1 431 * :Please enter a nickname\r\n");
		return (false);
	}
	if (choice & NOTLOG && !client->second->logged)
	{
		Utils::Send(client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :You are not logged in\r\n"));
		return (false);
	}
	return (true);
}

void Commands::pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass)
{
	client->second->cmd = client->second->cmd.substr(5);
	if (!checks(client, NOARGS))
		return ;

	if (!strcmp(client->second->cmd.c_str(), pass.c_str()))
		client->second->passed = true;
	else
	{
		Utils::Send(client->first, ":127.0.0.1 464 * :Password is incorect\r\n");
		for (int i = 0; pass[i]; ++i)
			std::cout << "'" << (int)client->second->cmd[i] << ":" << (int)pass[i] << "', ";
		std::cout << strcmp(client->second->cmd.c_str(), pass.c_str()) << std::endl;
	}
}

void Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(5);
	std::cout << "hey:" << client->second->cmd << ":hey" << std::endl;
	if (!checks(client, NOARGS | NOPASS))
		return ;

	client->second->nickname = client->second->cmd.substr(0, client->second->cmd.find_first_of("\r\n"));
}

void Commands::user(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(5);
	if (!checks(client, NOPASS | NONICK | NOARGS))
		return ;

	client->second->username = client->second->cmd.substr(0, client->second->cmd.find_first_of(" \r\n"));
	client->second->logged = true;
	std::cout << "client:" << "\n\tfd: " << client->second->fd
			<< "\n\tlogged: " << client->second->logged
			<< "\n\tnickname: " << client->second->nickname
			<< "\n\tpassed: " << client->second->passed
			<< "\n\tusername: " << client->second->username << std::endl;
	Utils::Send(client->first, std::string(":127.0.0.1 001 " + client->second->nickname + " :Welcome to ft_IRC\r\n"));
}

void	Commands::join(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(5);
	if (!checks(client, NOTLOG | NOARGS))
		return ;

	if (client->second->cmd[0] != '#')
	{
		std::cout << client->second->cmd[1] << std::endl;
		Utils::Send(client->first, std::string(":127.0.0.1 479 " + client->second->nickname + " :Illegal channel name\r\n"));
		return ;
	}
	std::string	channel = client->second->cmd.substr(1, client->second->cmd.find_first_of("\r\n") - 1);
	std::cout << "''" << channel << "''" << std::endl;
	if (Channels::isChannelReal(channel))
	{
		if (Channels::isInChannel((*Channels::find(channel)).second->users, client->second->nickname))
			Utils::Send(client->first, std::string(":127.0.0.1 443 " + client->second->nickname + " :Already in channel\r\n"));
		else
			(*Channels::find(channel)).second->users.insert(std::pair<t_clientInfo *, bool>(client->second, false));
	}
	else
		Channels::createChannel(channel, client->second);
}

void	Commands::who(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(5);
	if (!checks(client, NOTLOG | NOARGS))
		return ;
}
