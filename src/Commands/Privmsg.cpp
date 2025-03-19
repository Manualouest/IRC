/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:26:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/14 19:06:56 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::privmsgChannel(std::map<int, t_clientInfo*>::iterator client)
{
	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	if (!Channels::isChannelReal(channel))
	{
		Utils::Send(client->first, ":127.0.0.1 402 " + client->second->nickname + " :Channel doesn't exist.\r\n");
		std::cout << "'" << channel << "'" << std::endl;
		return ;
	}
	if (!Channels::isInChannel(Channels::find(channel)->second->users, client->second->nickname))
	{
		Utils::Send(client->first, ":127.0.0.1 404 " + client->second->nickname + " :Cannot send to channel.\r\n");
		return ;
	}
	Channels::sendMsg(channel, client->second, std::string(":" + client->second->nickname + " PRIVMSG " + client->second->cmdtoken.target
				 + " " + client->second->cmdtoken.args[0] + "\r\n"));
}

void	Commands::privmsgUser(std::map<int, t_clientInfo*>::iterator client, int (*f)(const std::string &))
{
	int user = f(client->second->cmdtoken.target);
	std::cout << "debug 2" << std::endl;
	if (user == -1)
	{
		Utils::Send(client->first, std::string(":127.0.0.1 401 " + client->second->nickname + ":This nickname doesn't exist.\r\n"));
		return ;
	}
	std::cout << "debug 3" << std::endl;
	std::cout << std::string(":" + client->second->nickname + " PRIVMSG " + client->second->cmdtoken.target + client->second->cmdtoken.args[0] + "\r\n") << std::endl;
	Utils::Send(user, std::string(":" + client->second->nickname + " PRIVMSG " + client->second->cmdtoken.target
				+ " " + client->second->cmdtoken.args[0] + "\r\n"));
}

void	Commands::privmsg(std::map<int, t_clientInfo*>::iterator client, int (*f)(const std::string &))
{
	std::cout << "debug 0" << std::endl;
	if (!checks(client, NOTLOG | NOTARG | NOARGS))
		return ;
	std::cout << "debug 1" << std::endl;
	if (client->second->cmdtoken.target[0] == '#')
		Commands::privmsgChannel(client);
	else
		privmsgUser(client, f);
}
