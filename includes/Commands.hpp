/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:12 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 14:26:23 by mbirou           ###   ########.fr       */
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
#define BADCHN 0b100000

class Commands
{
	public:
		static bool checks(std::map<int, t_clientInfo*>::iterator client, int choice);
		static void pass(std::map<int, t_clientInfo*>::iterator client, const std::string &pass);
		static void nick(std::map<int, t_clientInfo*>::iterator client);
		static void user(std::map<int, t_clientInfo*>::iterator client);
		static void	join(std::map<int, t_clientInfo*>::iterator client);
		static void	privmsg(std::map<int, t_clientInfo*>::iterator client, int (*f)(const std::string &));
		static void	part(std::map<int, t_clientInfo*>::iterator client);
		static void	quit(std::map<int, t_clientInfo*>::iterator client, void (*f)(t_clientInfo*));


	private:
		Commands();
		~Commands();

		static void	privmsgChannel(std::map<int, t_clientInfo*>::iterator client);
		static void	privmsgUser(std::map<int, t_clientInfo*>::iterator client, int (*f)(const std::string &));

		class Channels : public ChannelCommands
		{};
};
