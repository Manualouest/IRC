/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:26:08 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 13:13:59 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::user(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOPASS | NONICK | NOARGS))
		return ;
	if (client->second->cmdtoken.args.size() < 4)
	{
		Utils::Send(client->first, NOTENOUGHARGS(client->second->nickname));
		return ;
	}

	client->second->username = client->second->cmdtoken.args[0];
	client->second->realname = client->second->cmdtoken.args[3].substr(1, client->second->cmdtoken.args[3].length());
	client->second->logged = true;
	std::cout << "client:" << "\n\tfd: " << client->second->fd
			<< "\n\tlogged: " << client->second->logged
			<< "\n\tnickname: " << client->second->nickname
			<< "\n\tpassed: " << client->second->passed
			<< "\n\tusername: " << client->second->username << std::endl;
	Utils::Send(client->first, WELCOMEMSG(client->second->nickname));
}
