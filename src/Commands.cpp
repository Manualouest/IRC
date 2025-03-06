/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:11:44 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/06 16:25:25 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void Commands::pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass)
{
	if (!strcmp(client->second->cmd.substr(5, client->second->cmd.find_first_of("\r\n") - 2).c_str(), pass.c_str()))
		client->second->passed = true;
	else
		std::cout << "input pass: '" << client->second->cmd.substr(5, client->second->cmd.length() - 2) << "'" << ", '" << pass << std::endl;
}

void Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	if (!client->second->passed)
	{
		send(client->first, "Please insert password first\r\n", 31, MSG_DONTWAIT);
		return ;
	}
	client->second->nickname = client->second->cmd.substr(client->second->cmd.find_first_of(' '), client->second->cmd.find_first_of("\r\n") - 2);
}

void Commands::user(std::map<int, t_clientInfo*>::iterator client)
{
	if (!client->second->passed)
	{
		send(client->first, "Please insert password first\r\n", 31, MSG_DONTWAIT);
		return ;
	}
	if (client->second->nickname.empty())
	{
		send(client->first, "Please insert nickname first\r\n", 31, MSG_DONTWAIT);
		return ;
	}
	client->second->username = client->second->cmd.substr(client->second->cmd.find_first_of(':'), client->second->cmd.find_first_of("\r\n") - 2);
	client->second->logged = true;
	std::cout << "client:" << "\n\tfd: " << client->second->fd
			<< "\n\tlogged: " << client->second->logged
			<< "\n\tfd: " << client->second->nickname
			<< "\n\tfd: " << client->second->passed
			<< "\n\tfd: " << client->second->username << std::endl;
}

/*
PASS 1234
NICK test
USER test 0 * :test*/
