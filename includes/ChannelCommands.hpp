/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:39:01 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/01 13:44:04 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Includes.hpp>

class ChannelCommands
{
	public:
		static bool	isChannelReal(const std::string &channel);
		static void	createChannel(const std::string &channel, t_clientInfo *user);
		static bool	isInChannel(const std::string &channel, const std::string &nick);
		static void	sendMsg(const std::string &channel, const std::string &sender, const std::string &msg);
		static void	names(const std::string &channel, const t_clientInfo *sender);
		static void	partUser(const std::string &channel, t_clientInfo *user);
		static bool	isUserOp(const std::string &channel, const std::string &user);
		static void	opUser(const std::string &channel, const std::string &user);
		static void	dopUser(const std::string &channel, const std::string &user);
		static bool	isNickEq(std::pair<s_clientInfo *const, bool> client);
		static bool	isSpace(const std::string &channel);

		static std::map<t_clientInfo*, bool>::iterator					findUser(const std::string &channel, const std::string &nickname);
		static std::map<t_clientInfo*, bool>::iterator					channelUserEnd(const std::string &channel);
		static std::map<std::string, t_channelInfo*>::const_iterator	find(const std::string &channel);
		static std::map<std::string, t_channelInfo*>::const_iterator	begin();
		static std::map<std::string, t_channelInfo*>::const_iterator	end();

	private:
		ChannelCommands();
		~ChannelCommands();

		static std::map<std::string, t_channelInfo*>	_channels;
		static std::string								_searchnick;
};
