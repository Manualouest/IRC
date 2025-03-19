/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 11:34:39 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::join(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | BADCHN))
		return ;

	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	std::cout << "''" << channel << "''" << std::endl;
	if (Channels::isChannelReal(channel))
	{
		if (Channels::isInChannel(channel, client->second->nickname))
			Utils::Send(client->first, ALRINCHAN(client->second->nickname));
		else
			(*Channels::find(channel)).second->users.insert(std::pair<t_clientInfo *, bool>(client->second, false));
	}
	else
		Channels::createChannel(channel, client->second);
	Utils::Send(client->first, USERJOIN(client->second->nickname, channel));
	Channels::sendMsg(channel, client->second->nickname, USERJOIN(client->second->nickname, channel));
	Channels::names(channel, client->second);
}
