/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 09:16:08 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/19 10:53:28 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::quit(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOARGS))
		return ;

	std::map<int, int>	others;
	for (std::map<std::string, t_channelInfo*>::const_iterator channelIt = Channels::begin(); channelIt != Channels::end(); ++channelIt)
		for (std::map<t_clientInfo*, bool>::const_iterator clientIt = channelIt->second->users.begin(); clientIt != channelIt->second->users.end(); ++clientIt)
			others.insert(std::pair<int, int>(clientIt->first->fd, 0));
	for (std::map<int, int>::iterator fdsIt = others.begin(); fdsIt != others.end(); ++fdsIt)
		Utils::Send(fdsIt->first, QUITMSG(client->second->nickname, client->second->cmdtoken.args[0]));
	std::set<std::string>	channels;
	for (std::map<std::string, t_channelInfo*>::const_iterator it = Channels::begin(); it != Channels::end(); ++it)
		if (Channels::isInChannel(it->first, client->second->nickname))
			channels.insert(it->first);
	for (std::set<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		Channels::partUser(*it, client->second);
	Server::_delUser(client->second);
}
