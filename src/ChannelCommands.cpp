/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:42:07 by mbirou            #+#    #+#             */
/*   Updated: 2025/04/01 13:45:41 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommands.hpp>

std::map<std::string, t_channelInfo*>	ChannelCommands::_channels = std::map<std::string, t_channelInfo*>();
std::string								ChannelCommands::_searchnick = "";

bool	ChannelCommands::isChannelReal(const std::string &channel)
{
	for (std::map<std::string, t_channelInfo*>::const_iterator it = begin(); it != end(); ++it)
		if ((*it).first == channel)
			return true;
	return false;
}

void	ChannelCommands::createChannel(const std::string &channel, t_clientInfo *user)
{
	t_channelInfo	*channelfd = new t_channelInfo;

	channelfd->name = channel;
	channelfd->password = "";
	channelfd->topic = "";
	channelfd->isInviteOnly = false;
	channelfd->isTopicOPOnly = false;
	channelfd->limit = 0;
	channelfd->users.insert(std::pair<t_clientInfo*, bool>(user, true));
	_channels.insert(std::pair<std::string, t_channelInfo*>(channel, channelfd));
}

bool	ChannelCommands::isInChannel(const std::string &channel, const std::string &nick)
{
	std::map<t_clientInfo*, bool>	channelUsers = _channels.find(channel)->second->users;
	for (std::map<t_clientInfo*, bool>::iterator itClients = channelUsers.begin(); itClients != channelUsers.end(); ++itClients)
		if (itClients->first->nickname == nick)
			return true;
	return false;
}

void	ChannelCommands::sendMsg(const std::string &channel, const std::string &sender, const std::string &msg)
{
	std::map<std::string, t_channelInfo*>::const_iterator	channelfd = _channels.find(channel);
	int			messageLen = msg.length();
	const char	*msgC = msg.c_str();

	for (std::map<t_clientInfo*, bool>::iterator itClients = channelfd->second->users.begin(); itClients != channelfd->second->users.end(); ++itClients)
		if (itClients->first->nickname != sender)
			send(itClients->first->fd, msgC, messageLen, MSG_DONTWAIT);
}

void	ChannelCommands::names(const std::string &channel, const t_clientInfo *sender)
{
	std::map<std::string, t_channelInfo*>::const_iterator	channelfd = _channels.find(channel);

	Utils::Send(sender->fd, NAMESTART(sender->nickname, channel));
	for (std::map<t_clientInfo*, bool>::iterator itClients = channelfd->second->users.begin(); itClients != channelfd->second->users.end(); ++itClients)
		Utils::Send(sender->fd, NAMELIST(itClients));
	Utils::Send(sender->fd, NAMEEND(sender->nickname, channel));
}

void	ChannelCommands::partUser(const std::string &channel, t_clientInfo *user)
{
	_channels.find(channel)->second->users.erase(user);
	if (_channels.find(channel)->second->users.size() == 0)
	{
		delete _channels.find(channel)->second;
		_channels.erase(channel);
	}
}

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::find(const std::string &channel)
{
	return (_channels.find(channel));
}

bool	ChannelCommands::isUserOp(const std::string &channel, const std::string &user)
{
	if (findUser(channel, user) != channelUserEnd(channel))
		return (findUser(channel, user)->second);
	return (false);
}

void	ChannelCommands::opUser(const std::string &channel, const std::string &user)
{
	if (findUser(channel, user) != channelUserEnd(channel))
		findUser(channel, user)->second = true;
}

void	ChannelCommands::dopUser(const std::string &channel, const std::string &user)
{
	if (findUser(channel, user) != channelUserEnd(channel))
		findUser(channel, user)->second = false;
}

bool	ChannelCommands::isNickEq(std::pair<s_clientInfo *const, bool> client)
{
	return (client.first->nickname == _searchnick);
}

bool ChannelCommands::isSpace(const std::string &channel)
{
	if (_channels.find(channel)->second->limit == 0)
		return (true);
	return (_channels.find(channel)->second->limit - _channels.find(channel)->second->users.size() > 0);
}


std::map<t_clientInfo*, bool>::iterator	ChannelCommands::findUser(const std::string &channel, const std::string &user)
{
	_searchnick = user;
	return (std::find_if(_channels.find(channel)->second->users.begin(), _channels.find(channel)->second->users.end(), isNickEq));
}

std::map<t_clientInfo*, bool>::iterator	ChannelCommands::channelUserEnd(const std::string &channel)
{
	return (_channels.find(channel)->second->users.end());
}

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::begin()
{
	return (_channels.begin());
}

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::end()
{
	return (_channels.end());
}
