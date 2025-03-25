/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/25 09:51:59 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::tryJoin(std::map<int, t_clientInfo*>::iterator client, const std::string &channel)
{
	if (!Channels::isSpace(channel))
	{
		Utils::Send(client->first, ERRMODEL(client->second->nickname, channel));
		return ;
	}
	if (!Channels::find(channel)->second->password.empty())
	{
		if (!checks(client, NOARGS))
			return ;
		if (strcmp(Channels::find(channel)->second->password.c_str(), client->second->cmdtoken.args[0].c_str()))
		{
			Utils::Send(client->first, ERRMODEK(client->second->nickname, channel));
			return ;
		}
	}
	if (Channels::find(channel)->second->isInviteOnly)
	{
		if (Channels::find(channel)->second->invite.find(client->second->nickname) != Channels::find(channel)->second->invite.end())
			Channels::find(channel)->second->invite.erase(client->second->nickname);
		else
		{
			Utils::Send(client->first, ERRMODEI(client->second->nickname, channel));
			return ;
		}
	}
	(*Channels::find(channel)).second->users.insert(std::pair<t_clientInfo *, bool>(client->second, false));
	Utils::Send(client->first, USERJOIN(client->second->nickname, channel));
	Channels::sendMsg(channel, client->second->nickname, USERJOIN(client->second->nickname, channel));
	Channels::names(channel, client->second);
}

void	Commands::join(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | BADCHN))
		return ;

	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	std::cout << "''" << channel << "''" << std::endl;
	if (Channels::isChannelReal(channel))
	{
		if (Channels::isInChannel(channel, client->second->nickname))
			Utils::Send(client->first, ALRINCHAN(client->second->nickname, client->second->nickname, channel));
		else
			tryJoin(client, channel);
	}
	else
	{
		Channels::createChannel(channel, client->second);
		Utils::Send(client->first, USERJOIN(client->second->nickname, channel));
		Channels::sendMsg(channel, client->second->nickname, USERJOIN(client->second->nickname, channel));
		Channels::names(channel, client->second);
	}
}
