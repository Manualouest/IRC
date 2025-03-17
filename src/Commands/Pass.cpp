/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:25:51 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:26:50 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass)
{
	if (!checks(client, NOARGS))
		return ;

	if (!strcmp(client->second->cmdtoken.args[0].c_str(), pass.c_str()))
		client->second->passed = true;
	else
	{
		Utils::Send(client->first, ":127.0.0.1 464 * :Password is incorect\r\n");
		std::cout << "pass given: " << client->second->cmdtoken.args[0] << std::endl;
		for (int i = 0; pass[i]; ++i)
			std::cout << "'" << (int)client->second->cmdtoken.args[0][i] << ":" << (int)pass[i] << "', ";
		std::cout << strcmp(client->second->cmdtoken.args[0].c_str(), pass.c_str()) << std::endl;
	}
}