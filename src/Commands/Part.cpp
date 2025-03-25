/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 08:44:50 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/25 09:37:16 by derey            ###   ########.fr       */
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
		Utils::Send(client->first, CHANNOTREAL(client->second->nickname, channel));
		return ;
	}
	if (!Channels::isInChannel(channel, client->second->nickname))
	{
		Utils::Send(client->first, NOTONCHAN(client->second->nickname, channel));
		return ;
	}

	std::string message = USERPART(client->second->nickname, client->second->cmdtoken.target, client->second->cmdtoken.args[0]);
	Channels::sendMsg(channel, client->second->nickname, message);
	Utils::Send(client->second->fd, message);
	Channels::partUser(channel, client->second);
}
