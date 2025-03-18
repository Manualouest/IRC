/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:42:07 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/17 13:47:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ChannelCommands.hpp>

std::map<std::string, t_channelInfo*>	ChannelCommands::_channels = std::map<std::string, t_channelInfo*>();

bool	ChannelCommands::isChannelReal(const std::string &name)
{
	for (std::map<std::string, t_channelInfo*>::const_iterator it = begin(); it != end(); ++it)
		if ((*it).first == name)
			return true;
	return false;
}

void	ChannelCommands::createChannel(const std::string &name, t_clientInfo *user)
{
	t_channelInfo	*channel = new t_channelInfo;

	channel->name = name;
	channel->password = "";
	channel->topic = "";
	channel->isInviteOnly = false;
	channel->isTopicOPOnly = false;
	channel->limit = 0;
	std::cout << "channel create debug: '" << name << "'" << std::endl;
	channel->users.insert(std::pair<t_clientInfo*, bool>(user, true));
	_channels.insert(std::pair<std::string, t_channelInfo*>(name, channel));
}

bool	ChannelCommands::isInChannel(std::map<t_clientInfo*, bool> users, const std::string &nick)
{
	for (std::map<t_clientInfo*, bool>::iterator itClients = users.begin(); itClients != users.end(); ++itClients)
		if ((*itClients).first->nickname == nick)
			return true;
	return false;
}

void	ChannelCommands::sendMsg(const std::string &name, const t_clientInfo *sender, const std::string &msg)
{
	std::map<std::string, t_channelInfo*>::const_iterator	channel = _channels.find(name);
	int			messageLen = msg.length();
	const char	*msgC = msg.c_str();

	for (std::map<t_clientInfo*, bool>::iterator itClients = channel->second->users.begin(); itClients != channel->second->users.end(); ++itClients)
		if ((*itClients).first->nickname != sender->nickname)
			send((*itClients).first->fd, msgC, messageLen, MSG_DONTWAIT);
}

void	ChannelCommands::names(const std::string &name, const t_clientInfo *sender)
{
	if (!isChannelReal(name))
	{
		Utils::Send(sender->fd, "Channel doesn't exist\r\n");
		return ;
	}

	std::map<std::string, t_channelInfo*>::const_iterator	channel = _channels.find(name);

	Utils::Send(sender->fd, std::string(":127.0.0.1 353 " + sender->nickname + " = #" + name + " :"));
	for (std::map<t_clientInfo*, bool>::iterator itClients = channel->second->users.begin(); itClients != channel->second->users.end(); ++itClients)
		Utils::Send(sender->fd, std::string(((*itClients).second ? " @" : " ") + (*itClients).first->nickname));
	Utils::Send(sender->fd, std::string("\r\n"));
	Utils::Send(sender->fd, std::string(":127.0.0.1 366 " + sender->nickname + " #" + name + " :End of /NAMES list\r\n"));
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

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::find(const std::string &name)
{
	return (_channels.find(name));
}

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::begin()
{
	return (_channels.begin());
}

std::map<std::string, t_channelInfo*>::const_iterator	ChannelCommands::end()
{
	return (_channels.end());
}
