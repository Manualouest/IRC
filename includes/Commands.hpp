/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:12:12 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 11:05:53 by mbatty           ###   ########.fr       */
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
		static void	mode(std::map<int, t_clientInfo*>::iterator client);
		static void	quit(std::map<int, t_clientInfo*>::iterator client, void (*f)(t_clientInfo*));
		static void topic(std::map<int, t_clientInfo*>::iterator client);

		static void	modeOperator(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args);
		static void	modeKeypass(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args);
		static void	modeUserLimit(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args);
		static void	modeTopicAccess(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args);
		static void	modeInviteOnly(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args);
		static void	returnModeInfo(std::string channel, std::map<int, t_clientInfo*>::iterator client);
	private:
		Commands();
		~Commands();

		static void	privmsgChannel(std::map<int, t_clientInfo*>::iterator client);
		static void	privmsgUser(std::map<int, t_clientInfo*>::iterator client, int (*f)(const std::string &));

		class Channels : public ChannelCommands
		{};
};
