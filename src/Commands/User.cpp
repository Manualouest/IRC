/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:26:08 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/02 08:41:59 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::user(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOPASS | NOARGS))
		return ;
	if (!checks(client, NONICK))
	{
		client->second->usertoken.cmd = client->second->cmdtoken.cmd;
		client->second->usertoken.args = client->second->cmdtoken.args;
		client->second->usertoken.target = client->second->cmdtoken.target;
		return ;
	}
	if (client->second->cmdtoken.args.size() < 4)
	{
		Utils::Send(client->first, NOTENOUGHARGS(client->second->nickname));
		return ;
	}

	client->second->username = client->second->cmdtoken.args[0];
	client->second->realname = client->second->cmdtoken.args[3].substr(client->second->cmdtoken.args[1][0] == ':', client->second->cmdtoken.args[3].length());
	client->second->logged = true;
	PRINT "client:" AND "\n\tfd: " AND client->second->fd
			AND "\n\tlogged: " AND client->second->logged
			AND "\n\tnickname: " AND client->second->nickname
			AND "\n\tpassed: " AND client->second->passed
			AND "\n\tusername: " AND client->second->username AND std::endl;
	NEWL;
	Utils::Send(client->first, WELCOMEMSG(client->second->nickname));
}
