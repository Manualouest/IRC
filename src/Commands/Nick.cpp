/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:01 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/01 13:38:52 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOARGS | NOPASS))
		return ;

	if (Server::_findUser(client->second->cmdtoken.args[0]) == -1)
	{
		for (std::map<std::string, t_channelInfo*>::const_iterator it = Channels::begin(); it != Channels::end(); it++)
		{
			if (Channels::isInChannel(it->second->name, client->second->cmdtoken.args[0]))
				Channels::sendMsg(it->first, client->second->nickname, std::string(":" + client->second->nickname + " NICK " + client->second->cmdtoken.args[0] + "\r\n"));
		}
		client->second->nickname = client->second->cmdtoken.args[0];
		if (!client->second->usertoken.cmd.empty())
		{
			client->second->cmdtoken.cmd = client->second->usertoken.cmd;
			client->second->cmdtoken.args = client->second->usertoken.args;
			client->second->cmdtoken.target = client->second->usertoken.target;
			client->second->usertoken.cmd = "";
			user(client);
		}
	}
	else
		Utils::Send(client->first, std::string(":127.0.0.1 433 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " :Nickname is already in use\r\n"));
}
