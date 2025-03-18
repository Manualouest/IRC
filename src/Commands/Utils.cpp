/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:42 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 14:14:41 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

bool	Commands::checks(std::map<int, t_clientInfo*>::iterator client, int choice)
{
	if (choice & NOARGS && !client->second->cmdtoken.args.size())
	{
		Utils::Send(client->first, NOTENOUGHARGS(client->second->nickname));
		return (false);
	}
	if (choice & NOPASS && !client->second->passed)
	{
		Utils::Send(client->first, NOPASSGIVEN(client->second->nickname));
		return (false);
	}
	if (choice & NONICK && client->second->nickname == "*")
	{
		Utils::Send(client->first, NONICKGIVEN(client->second->nickname));
		return (false);
	}
	if (choice & NOTLOG && !client->second->logged)
	{
		Utils::Send(client->first, UNKNOWN(client->second->nickname, "You are not logged in"));
		return (false);
	}
	if (choice & NOTARG && !client->second->cmdtoken.target.length())
	{
		Utils::Send(client->first, UNKNOWN(client->second->nickname, "No target given"));
		return (false);
	}
	if (choice & BADCHN && client->second->cmdtoken.target[0] != '#')
	{
		Utils::Send(client->first, BADCHANNAME(client->second->nickname));
		return (false);
	}
	return (true);
}
