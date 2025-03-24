/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:01 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/24 12:48:51 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "hey:" << client->second->cmdtoken.args[0] << ":hey" << std::endl;
	if (!checks(client, NOARGS | NOPASS))
		return ;

	if (Server::_findUser(client->second->cmdtoken.args[0]) == -1)
	{
		Utils::Send(client->first, std::string(":" + client->second->nickname + " NICK :" + client->second->cmdtoken.args[0] + "\r\n"));
		for (std::map<std::string, t_channelInfo*>::const_iterator it = Channels::begin(); it != Channels::end(); it++)
			Channels::sendMsg(it->first, client->second->nickname, std::string(":" + client->second->nickname + " NICK " + client->second->cmdtoken.args[0] + "\r\n"));
		client->second->nickname = client->second->cmdtoken.args[0];
	}
	else
		Utils::Send(client->first, std::string(":127.0.0.1 433 " + client->second->nickname + " " + client->second->cmdtoken.args[0] + " :Nickname is already in use\r\n"));
}
