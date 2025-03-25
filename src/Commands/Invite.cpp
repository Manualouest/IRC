/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:17:30 by derey             #+#    #+#             */
/*   Updated: 2025/03/25 09:47:07 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::invite(std::map<int, t_clientInfo*>::iterator client)
{
	std::string	channel = client->second->cmdtoken.args[1].substr(1,client->second->cmdtoken.args[1].length());
	if (!Channels::isChannelReal(channel))
	{
		Utils::Send(client->first, CHANNOTREAL(client->second->nickname, channel));
		return ;
	}
	if (!Channels::isInChannel(channel, client->second->nickname))
	{
		Utils::Send(client->first, NOTONCHAN(client->second->nickname, channel));
		return ;
	}
	if (Server::_findUser(client->second->cmdtoken.args[0]) == -1)
	{
		Utils::Send(client->first, NOSUCHNICK(client->second->nickname, client->second->cmdtoken.args[0]));
		return ;
	}
	if (Channels::isInChannel(channel, client->second->cmdtoken.args[0]))
	{
		Utils::Send(client->first, ALRINCHAN(client->second->nickname, client->second->cmdtoken.args[0], channel));
		return ;
	}
	if (!Channels::find(channel)->second->users.find(client->second)->second)
	{
		Utils::Send(client->first, NOTOPERATOR(client->second->nickname, channel));
		return ;
	}
	Utils::Send(client->first, ":127.0.0.1 341 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " #" + channel + "\r\n");
	Utils::Send(Server::_findUser(client->second->cmdtoken.args[0]), "You have been invited to #" + channel + " by " + client->second->nickname + "\r\n");
	Channels::find(channel)->second->invite.insert(client->second->cmdtoken.args[0]);
}
