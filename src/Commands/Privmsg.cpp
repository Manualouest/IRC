/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:26:04 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:33:35 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	privmsgChannel(std::map<int, t_clientInfo*>::iterator client)
{

}

void	Commands::privmsg(std::map<int, t_clientInfo*>::iterator client)
{
	if (checks(client, NOTLOG | NOTARG | NOARGS))
		return ;
	if (client->second->cmdtoken.target[0] != '#')
	{
		std::cout << client->second->cmdtoken.target[1] << std::endl;
		Utils::Send(client->first, std::string(":127.0.0.1 479 " + client->second->nickname + " :Illegal channel name\r\n"));
		return ;
	}
	std::string	channel = client->second->cmdtoken.target.substr(1, client->second->cmdtoken.target.length());
	if (!Channels::isChannelReal(client->second->cmdtoken.target))
	{
		
		return ;
	}
	if (!Channels::isInChannel(Channels::find(client->second->cmdtoken.target)->second->users, client->second->nickname))
	{

		return ;
	}
}

// PRIVMSG #test :test