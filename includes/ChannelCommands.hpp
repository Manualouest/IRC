/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:39:01 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/11 08:33:59 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Includes.hpp>

class ChannelCommands
{
	public:
		static bool	isChannelReal(const std::string &name);
		static void	createChannel(const std::string &name, t_clientInfo *user);
		static bool	isInChannel(std::map<t_clientInfo*, bool> users, const std::string &nick);
		static void	sendMsg(const std::string &name, const t_clientInfo *sender, const std::string &msg);
		static void	who(const std::string &name, const t_clientInfo *sender);

		static std::map<std::string, t_channelInfo*>::const_iterator	find(const std::string &name);
		static std::map<std::string, t_channelInfo*>::const_iterator	begin();
		static std::map<std::string, t_channelInfo*>::const_iterator	end();

	private:
		ChannelCommands();
		~ChannelCommands();
		static std::map<std::string, t_channelInfo*>	_channels;
};