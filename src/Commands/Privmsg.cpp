/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:26:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/02 10:57:44 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::privmsgChannel(std::map<int, t_clientInfo*>::iterator client)
{
	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	if (!Channels::isChannelReal(channel))
	{
		Utils::Send(client->first, CHANNOTREAL(client->second->nickname, channel));
		return ;
	}
	if (!Channels::isInChannel(channel, client->second->nickname))
	{
		Utils::Send(client->first, CANTSENDTOCHAN(client->second->nickname, channel));
		return ;
	}
	Channels::sendMsg(channel, client->second->nickname, PRIVMSG(client->second->nickname, client->second->cmdtoken.target, client->second->cmdtoken.args[0]));
}

void	Commands::privmsgUser(std::map<int, t_clientInfo*>::iterator client)
{
	int user = Server::_findUser(client->second->cmdtoken.target);
	if (user == -1)
	{
		Utils::Send(client->first, NOSUCHNICK(client->second->nickname, client->second->cmdtoken.target));
		return ;
	}
	Utils::Send(user, PRIVMSG(client->second->nickname, client->second->cmdtoken.target, client->second->cmdtoken.args[0]));
}

void	Commands::privmsg(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | NOARGS))
		return ;
	if (client->second->cmdtoken.target[0] == '#')
		privmsgChannel(client);
	else
		privmsgUser(client);
}
