/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:42 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:27:17 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

bool	Commands::checks(std::map<int, t_clientInfo*>::iterator client, int choice)
{
	if (choice & NOARGS && !client->second->cmdtoken.args.size())
	{
		Utils::Send(client->first, ":127.0.0.1 421 * :No argument given\r\n");
		return (false);
	}
	if (choice & NOPASS && !client->second->passed)
	{
		Utils::Send(client->first, ":127.0.0.1 464 * :Password was not given\r\n");
		return (false);
	}
	if (choice & NONICK && client->second->nickname.empty())
	{
		Utils::Send(client->first, ":127.0.0.1 431 * :Please enter a nickname\r\n");
		return (false);
	}
	if (choice & NOTLOG && !client->second->logged)
	{
		Utils::Send(client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :You are not logged in\r\n"));
		return (false);
	}
	if (choice & NOTARG && !client->second->cmdtoken.target.size())
	{
		Utils::Send(client->first, std::string(":127.0.0.1 421 " + client->second->nickname + " :No target given\r\n"));
		return (false);
	}
	return (true);
}
