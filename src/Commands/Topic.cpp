/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:57:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/04/02 08:45:07 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

void Commands::topic(std::map<int, t_clientInfo*>::iterator client)
{
	if (!checks(client, NOTLOG | NOTARG | BADCHN))
		return ;

	std::vector<std::string>	args = client->second->cmdtoken.args;
	std::string					channel = client->second->cmdtoken.target.substr(1);
	std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);

	if (Channels::isChannelReal(channel))
	{
		if (args.size() == 0)
		{
			if (chan->second->topic.empty())
				Utils::Send(client->first, std::string(":127.0.0.1 331 " + client->second->nickname + " #" + channel + " :No topic is not set\r\n"));	
			else
				Utils::Send(client->first, std::string(":127.0.0.1 332 " + client->second->nickname + " #" + channel + " :" + chan->second->topic + "\r\n"));	
		}
		else
		{
			if (chan->second->isTopicOPOnly && !ChannelCommands::isUserOp(channel, client->second->nickname))
				Utils::Send(client->first, std::string(":127.0.0.1 482 " + client->second->nickname + " :You are not channel operator\r\n"));	
			else
			{
				chan->second->topic = args[0].substr(args[0][0] == ':');
				ChannelCommands::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " TOPIC #" + channel + " :" + chan->second->topic + "\r\n"));
				Utils::Send(client->first, std::string(":" + client->second->nickname + " TOPIC #" + channel + " :" + chan->second->topic + "\r\n"));
			}
		}
	}
	else
		Utils::Send(client->first, std::string(":127.0.0.1 403 " + client->second->nickname + " :Channel does not exist\r\n"));	
}
