/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 09:16:08 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 11:02:32 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::quit(std::map<int, t_clientInfo*>::iterator client, void (*f)(t_clientInfo*))
{
	if (!checks(client, NOTLOG | NOARGS))
		return ;

	std::map<int, int>	others;
	for (std::map<std::string, t_channelInfo*>::const_iterator channelIt = Channels::begin(); channelIt != Channels::end(); ++channelIt)
		for (std::map<t_clientInfo*, bool>::const_iterator clientIt = channelIt->second->users.begin(); clientIt != channelIt->second->users.end(); ++clientIt)
			others.insert(std::pair<int, int>(clientIt->first->fd, 0));
	for (std::map<int, int>::iterator fdsIt = others.begin(); fdsIt != others.end(); ++fdsIt)
		Utils::Send(fdsIt->first, std::string(":" + client->second->nickname + " QUIT :Quit: "
			+ client->second->cmdtoken.args[0].substr((client->second->cmdtoken.args[0].find(':') != std::string::npos ? client->second->cmdtoken.args[0].find(':') : 0)) + "\r\n"));
	std::set<std::string>	channels;
	for (std::map<std::string, t_channelInfo*>::const_iterator it = Channels::begin(); it != Channels::end(); ++it)
		if (Channels::isInChannel(it->first, client->second->nickname))
			channels.insert(it->first);
	for (std::set<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		Channels::partUser(*it, client->second);
	f(client->second);
}
