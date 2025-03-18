/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 08:44:50 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 10:31:03 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::part(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | NOARGS | BADCHN))
		return ;

	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	if (!Channels::isChannelReal(channel))
	{
		Utils::Send(client->first, ":127.0.0.1 403 " + client->second->nickname + " :Channel doesn't exist.\r\n");
		return ;
	}
	if (!Channels::isInChannel(channel, client->second->nickname))
	{
		Utils::Send(client->first, ":127.0.0.1 442 " + client->second->nickname + " :Not on channel.\r\n");
		return ;
	}

	std::string message = std::string(":" + client->second->nickname + " PART "
						+ client->second->cmdtoken.target + " " + client->second->cmdtoken.args[0] + "\r\n");
	Channels::sendMsg(channel, client->second->nickname, message);
	Utils::Send(client->second->fd, message);
	Channels::partUser(channel, client->second);
}
