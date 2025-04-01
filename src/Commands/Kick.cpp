/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: derey <derey@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 13:52:09 by derey             #+#    #+#             */
/*   Updated: 2025/04/01 17:14:45 by derey            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::kick(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | NOARGS | BADCHN))
		return ;

	std::string	channel =  client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	std::string user = client->second->cmdtoken.args[0];
	std::string arg = client->second->nickname;
	if (client->second->cmdtoken.args.size() > 1)	
		arg = client->second->cmdtoken.args[1].substr(1, client->second->cmdtoken.args[1].length());

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
	if (!Channels::isInChannel(channel, user))
	{
		Utils::Send(client->first, USERNOTINCHANNEL(client->second->nickname, client->second->cmdtoken.target));	
		return ;
	}
	if (!Channels::find(channel)->second->users.find(client->second)->second)
	{
		Utils::Send(client->first, NOTOPERATOR(client->second->nickname, channel));
		return ;
	}
	Channels::sendMsg(channel, client->second->nickname, USERKICK(client->second->nickname, channel, user, arg));
	Channels::partUser(channel, Channels::findUser(channel, user)->first);
	if (Channels::isChannelReal(channel))
		Channels::sendMsg(channel, client->second->nickname, HASKICKED(client->second->nickname, channel, user, arg));
	Utils::Send(client->first, USERKICK(client->second->nickname, channel, user, arg));
	Utils::Send(Server::_findUser(user), BEENKICKED(client->second->nickname, channel, arg));
}
