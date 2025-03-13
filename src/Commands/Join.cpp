/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:08:37 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::join(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG))
		return ;

	if (client->second->cmdtoken.target[0] != '#')
	{
		std::cout << client->second->cmdtoken.target[1] << std::endl;
		Utils::Send(client->first, std::string(":127.0.0.1 479 " + client->second->nickname + " :Illegal channel name\r\n"));
		return ;
	}
	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
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
	Utils::Send(client->first, std::string(":" + client->second->nickname + " JOIN #" + channel + "\r\n"));
	Channels::sendMsg(channel, client->second, std::string(":" + client->second->nickname + " JOIN #" + channel + "\r\n"));
	Channels::names(channel, client->second);
}
