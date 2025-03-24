/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */	
/*   Created: 2025/03/17 09:17:30 by derey             #+#    #+#             */
/*   Updated: 2025/03/17 13:56:59 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::invite(std::map<int, t_clientInfo*>::iterator client)
{
	std::string	channel = client->second->cmdtoken.args[1].substr(1,client->second->cmdtoken.args[1].length());
	if (!Channels::isChannelReal(channel))
	{
		Utils::Send(client->first, ":127.0.0.1 403 " + client->second->nickname + " #" + channel + " :Channel doesn't exist.\r\n");
		return ;
	}
	if (!Channels::isInChannel(Channels::find(channel)->second->users, client->second->nickname))
	{
		Utils::Send(client->first, ":127.0.0.1 442 " + client->second->nickname + " #" + channel + " :You're not on that channel.\r\n");
		return ;
	}
	if (Server::_findUser(client->second->cmdtoken.args[0]) == -1)
	{
		Utils::Send(client->first, ":127.0.0.1 401 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " :No such nick.\r\n");
		return ;
	}
	if (Channels::isInChannel(Channels::find(channel)->second->users, client->second->cmdtoken.args[0]))
	{
		Utils::Send(client->first, ":127.0.0.1 443 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " #" + channel + " :is already on channel.\r\n");
		return ;
	}
	if (!Channels::find(channel)->second->users.find(client->second)->second)
	{
		Utils::Send(client->first, ":127.0.0.1 482 " + client->second->nickname + " #" + channel + " :You're not channel operator.\r\n");
		return ;
	}
	Utils::Send(client->first, ":127.0.0.1 341 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " #" + channel + "\r\n");
	Utils::Send(Server::_findUser(client->second->cmdtoken.args[0]), "You have been invited to #" + channel + " by " + client->second->nickname + "\r\n");
	Channels::find(channel)->second->invite.insert(client->second->cmdtoken.args[0]);
}
