/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:27:01 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:26:52 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void	Commands::nick(std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "hey:" << client->second->cmdtoken.args[0] << ":hey" << std::endl;
	if (!checks(client, NOARGS | NOPASS))
		return ;

	client->second->nickname = client->second->cmdtoken.args[0];
}
