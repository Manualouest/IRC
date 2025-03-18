/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:42:07 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/18 11:16:17 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommands.hpp>

std::map<std::string, t_channelInfo*>	ChannelCommands::_channels = std::map<std::string, t_channelInfo*>();

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
	channelfd->limit		= 0;
	std::cout << "channel create debug: '" << channel << "'" << std::endl;
	channelfd->users.insert(std::pair<t_clientInfo*, bool>(user, true));
	_channels.insert(std::pair<std::string, t_channelInfo*>(channel, channelfd));
}

bool	ChannelCommands::isInChannel(const std::string &channel, const std::string &nick)
{
	std::map<t_clientInfo*, bool>	channelUsers = _channels.find(channel)->second->users;
	for (std::map<t_clientInfo*, bool>::iterator itClients = channelUsers.begin(); itClients != channelUsers.end(); ++itClients)
		if ((*itClients).first->nickname == nick)
			return true;
	return false;
}

void	ChannelCommands::sendMsg(const std::string &channel, const std::string &sender, const std::string &msg)
{
	std::map<std::string, t_channelInfo*>::const_iterator	channelfd = _channels.find(channel);
	int			messageLen = msg.length();
	const char	*msgC = msg.c_str();

	for (std::map<t_clientInfo*, bool>::iterator itClients = channelfd->second->users.begin(); itClients != channelfd->second->users.end(); ++itClients)
		if ((*itClients).first->nickname != sender)
			send((*itClients).first->fd, msgC, messageLen, MSG_DONTWAIT);
}

void	ChannelCommands::names(const std::string &channel, const t_clientInfo *sender)
{
	std::map<std::string, t_channelInfo*>::const_iterator	channelfd = _channels.find(channel);

	Utils::Send(sender->fd, std::string(":127.0.0.1 353 " + sender->nickname + " = #" + channel + " :"));
	for (std::map<t_clientInfo*, bool>::iterator itClients = channelfd->second->users.begin(); itClients != channelfd->second->users.end(); ++itClients)
		Utils::Send(sender->fd, std::string(((*itClients).second ? " @" : " ") + (*itClients).first->nickname));
	Utils::Send(sender->fd, std::string("\r\n"));
	Utils::Send(sender->fd, std::string(":127.0.0.1 366 " + sender->nickname + " #" + channel + " :End of /NAMES list\r\n"));
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
		return (ChannelCommands::findUser(channel, user)->second);
	return (false);
}

std::map<t_clientInfo*, bool>::const_iterator	ChannelCommands::findUser(const std::string &channel, const std::string &user)
{
	std::map<t_clientInfo *, bool> channelUsers = _channels.find(channel)->second->users;
	for (std::map<t_clientInfo*, bool>::const_iterator it = channelUsers.begin(); it != channelUsers.end(); ++it)
		if (it->first->nickname == user)
			return (it);
	return (channelUserEnd(channel));
}

std::map<t_clientInfo*, bool>::const_iterator	ChannelCommands::channelUserEnd(const std::string &channel)
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
