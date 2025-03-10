/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:11:44 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/10 17:37:57 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void Commands::pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass)
{
	if (6 >= client->second->cmd.length() - 1)
	{
		send (client->first, ":127.0.0.1 421 * :No argument given\r\n", 38, MSG_DONTWAIT);
		return ;
	}
	if (!strcmp(client->second->cmd.substr(5).c_str(), pass.c_str()))
		client->second->passed = true;
	else
	{
		send(client->first, ":127.0.0.1 464 * :Password is incorect\r\n", 41, MSG_DONTWAIT);
		for (int i = 0; pass[i]; ++i)
			std::cout << "'" << (int)client->second->cmd.substr(5)[i] << ":" << (int)pass[i] << "', ";
		std::cout << strcmp(client->second->cmd.substr(5).c_str(), pass.c_str()) << std::endl;
	}
}

void Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	if (6 >= client->second->cmd.length() - 1)
	{
		send (client->first, ":127.0.0.1 421 * :No argument given\r\n", 38, MSG_DONTWAIT);
		return ;
	}
	if (!client->second->passed)
	{
		send(client->first, ":127.0.0.1 464 * :Password was not given\r\n", 43, MSG_DONTWAIT);
		return ;
	}
	client->second->nickname = client->second->cmd.substr(client->second->cmd.find_first_of(' ') + 1, client->second->cmd.find_first_of("\r\n") - client->second->cmd.find_first_of(' ') -1);
}

void Commands::user(std::map<int, t_clientInfo*>::iterator client)
{
	if (!client->second->passed)
	{
		send(client->first, ":127.0.0.1 464 * :Password was not given\r\n", 43, MSG_DONTWAIT);
		return ;
	}
	if (client->second->nickname.empty())
	{
		send(client->first, ":127.0.0.1 431 * :Please enter a nickname\r\n", 44, MSG_DONTWAIT);
		return ;
	}
	if (6 >= client->second->cmd.length() - 1)
	{
		send (client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :No argument given\r\n").c_str(), 16 + 22 + client->second->nickname.length(), MSG_DONTWAIT);
		return ;
	}
	client->second->username = client->second->cmd.substr(client->second->cmd.find_first_of(' ') + 1, client->second->cmd.find_first_of(" \r\n", client->second->cmd.find_first_of(' ') + 1) - client->second->cmd.find_first_of(' ') - 1);
	client->second->logged = true;
	std::cout << "client:" << "\n\tfd: " << client->second->fd
			<< "\n\tlogged: " << client->second->logged
			<< "\n\tnickname: " << client->second->nickname
			<< "\n\tpassed: " << client->second->passed
			<< "\n\tusername: " << client->second->username << std::endl;
	send(client->first, std::string(":127.0.0.1 001 " + client->second->nickname + " :Welcome to ft_IRC\r\n").c_str(), 16 + 22 + client->second->nickname.length(), MSG_DONTWAIT);
}

void	Commands::join(std::map<int, t_clientInfo*>::iterator client)
{
	if (!client->second->logged)
	{
		send (client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :You are not logged in\r\n").c_str(), 16 + 26 + client->second->nickname.length(), MSG_DONTWAIT);
		return ;
	}
	if (client->second->cmd.length() <= 8)
	{
		send (client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :No argument given\r\n").c_str(), 16 + 22 + client->second->nickname.length(), MSG_DONTWAIT);
		return ;
	}
	std::string	channel = client->second->cmd.substr(6, client->second->cmd.find_first_of("\r\n") -6);
	std::cout << "''" << channel << "''" << std::endl;
	if (Channels::isChannelReal(channel))
	{
		if (Channels::isInChannel((*Channels::find(channel)).second->users, client->second->nickname))
			send (client->first, std::string(":127.0.0.1 443 " + client->second->nickname + " :Already in channel\r\n").c_str(), 16 + 23 + client->second->nickname.length(), MSG_DONTWAIT);
		else
			(*Channels::find(channel)).second->users.insert(std::pair<t_clientInfo *, bool>(client->second, false));
	}
	else
		Channels::createChannel(channel, client->second);
}
