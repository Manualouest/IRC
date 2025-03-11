/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <manutea.birou@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:42:07 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/10 20:23:02 by mbirou           ###   ########.fr       */
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
	channel->users.insert(std::pair<t_clientInfo*, bool>(user, true));
	_channels.insert(std::pair<std::string, t_channelInfo*>(name, channel));
	std::cout << std::string(":" + user->nickname + " JOIN #" + name).c_str() << ";" << std::string(":" + user->nickname + " JOIN #" + name + "\r\n").length() << ", " << 2 + user->nickname.length() + 8 + name.length() + 3 - 3 << std::endl;
	send(user->fd, std::string(":" + user->nickname + " JOIN #" + name + "\r\n").c_str(), 2 + user->nickname.length() + 8 + name.length() + 3 - 3, MSG_DONTWAIT);
	sendMsg(name, user, std::string(":" + user->nickname + " JOIN #" + name + "\r\n"));
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
	if (!isChannelReal(name))
	{
		send(sender->fd, "Channel doesn't exist\r\n", 24, MSG_DONTWAIT);
		return ;
	}

	std::map<std::string, t_channelInfo*>::const_iterator	channel = _channels.find(name);
	int			messageLen = msg.length();
	const char	*msgC = msg.c_str();

	for (std::map<t_clientInfo*, bool>::iterator itClients = channel->second->users.begin(); itClients != channel->second->users.end(); ++itClients)
		if ((*itClients).first->nickname != sender->nickname)
			send((*itClients).first->fd, msgC, messageLen, MSG_DONTWAIT);
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

