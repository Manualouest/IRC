/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelCommands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:42:07 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/11 13:15:44 by mbirou           ###   ########.fr       */
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
	Utils::Send(user->fd, std::string(":" + user->nickname + " JOIN #" + name + "\r\n"));
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
		Utils::Send(sender->fd, "Channel doesn't exist\r\n");
		return ;
	}

	std::map<std::string, t_channelInfo*>::const_iterator	channel = _channels.find(name);
	int			messageLen = msg.length();
	const char	*msgC = msg.c_str();

	for (std::map<t_clientInfo*, bool>::iterator itClients = channel->second->users.begin(); itClients != channel->second->users.end(); ++itClients)
		if ((*itClients).first->nickname != sender->nickname)
			send((*itClients).first->fd, msgC, messageLen, MSG_DONTWAIT);
}

void	ChannelCommands::who(const std::string &name, const t_clientInfo *sender)
{
	if (!isChannelReal(name))
	{
		Utils::Send(sender->fd, "Channel doesn't exist\r\n");
		return ;
	}

	std::map<std::string, t_channelInfo*>::const_iterator	channel = _channels.find(name);

	for (std::map<t_clientInfo*, bool>::iterator itClients = channel->second->users.begin(); itClients != channel->second->users.end(); ++itClients)
		if ((*itClients).first->nickname != sender->nickname)
			Utils::Send(sender->fd, std::string(":127.0.0.1 352 " + (*itClients).first->nickname + " " + name + " ~"
				+ (*itClients).first->username + "127.0.0.1 127.0.0.1 " + (*itClients).first->nickname + " H :0 " + (*itClients).first->realname));
	Utils::Send(sender->fd, std::string(":127.0.0.1 315 " + sender->nickname + " " + name + " :End of /WHO list\r\n"));
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
