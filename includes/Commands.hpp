/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:12 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/13 18:27:26 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Includes.hpp>
#include <ChannelCommands.hpp>

#define NOARGS 0b1
#define NOPASS 0b10
#define NONICK 0b100
#define NOTLOG 0b1000
#define NOTARG 0b10000

class Commands
{
	public:
		static bool checks(std::map<int, t_clientInfo*>::iterator client, int choice);
		static void pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass);
		static void nick(std::map<int, t_clientInfo*>::iterator client);
		static void user(std::map<int, t_clientInfo*>::iterator client);
		static void	join(std::map<int, t_clientInfo*>::iterator client);
		static void	privmsg(std::map<int, t_clientInfo*>::iterator client);


	private:
		Commands();
		~Commands();

		class Channels : public ChannelCommands
		{};
};
