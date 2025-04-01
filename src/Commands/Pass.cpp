/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:25:51 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/01 13:45:04 by mbirou           ###   ########.fr       */
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
		Utils::Send(client->first, WRONGPASS(client->second->nickname));
}