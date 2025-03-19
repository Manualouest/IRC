/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:41:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/03/19 14:46:17 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>

static std::map<std::string, char> modeGetFlags(std::string modeChar)
{
	char						curMod = '+';
	std::map<std::string, char>	mod;

	for (int i = 0; modeChar[i]; i++)
	{
		if (modeChar[i] == '\n' || modeChar[i] == ' ')
			break ;
		if (modeChar[i] == '-' || modeChar[i] == '+')
		{
			curMod = modeChar[i];
			continue ;
		}
		if (mod.find(modeChar.substr(i, 1)) == mod.end())
			mod.insert(std::make_pair(modeChar.substr(i, 1), curMod));
	}
	return (mod);
}

void	Commands::modeOperator(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args)
{
	if (!args.size())
		return ;
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		if (Channels::findUser(channel, args[0]) == Channels::channelUserEnd(channel))
		{
			Utils::Send(client->first, std::string(":127.0.0.1 401 " + client->second->nickname + " " + args[0] + " :No such nick/channel\r\n"));	
			return ;
		}
		if (!Channels::isInChannel(chan->first, args[0]))
		{
			Utils::Send(client->first, std::string(":127.0.0.1 441 " + client->second->nickname + " " + args[0] + " :They aren't on that channel\r\n"));	
			return ;
		}
		Channels::opUser(channel, args[0]);
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " +o " + args[0] + "\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " +o " + args[0] + "\r\n"));
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		if (Channels::findUser(channel, args[0]) == Channels::channelUserEnd(channel))
		{
			Utils::Send(client->first, std::string(":127.0.0.1 401 " + client->second->nickname + " " + args[0] + " :No such nick/channel\r\n"));	
			return ;
		}
		if (!Channels::isInChannel(chan->first, args[0]))
		{
			Utils::Send(client->first, std::string(":127.0.0.1 441 " + client->second->nickname + " " + args[0] + " :They aren't on that channel\r\n"));	
			return ;
		}
		Channels::dopUser(channel, args[0]);
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " -o " + args[0] + "\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " -o " + args[0] + "\r\n"));
		args.erase(args.begin());
	}
}

void	Commands::modeKeypass(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args)
{
	if (mod == '+')
	{
		if (!args.size())
			return ;
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->password = args[0];
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " +k " + args[0] + "\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " +k " + args[0] + "\r\n"));
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->password = "";
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " -k\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " -k\r\n"));
	}
}

void	Commands::modeUserLimit(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args)
{
	if (mod == '+')
	{
		if (!args.size() || std::atoi(args[0].c_str()) <= 0)
			return ;
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->limit = std::atoi(args[0].c_str());
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " +l " + args[0] + "\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " +l " + args[0] + "\r\n"));
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->limit = 0;
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " -l\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " -l\r\n"));
	}
}

void	Commands::modeTopicAccess(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args)
{
	(void)args;
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isTopicOPOnly = true;
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " +t\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " +t\r\n"));
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isTopicOPOnly = false;
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " -t\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " -t\r\n"));
	}
}

void	Commands::modeInviteOnly(std::map<int, t_clientInfo*>::iterator client, char mod, std::string channel, std::vector<std::string> &args)
{
	(void)args;
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isInviteOnly = true;
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " +i\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " +i\r\n"));
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isInviteOnly = false;
		Channels::sendMsg(channel, client->second->nickname, std::string(":" + client->second->nickname + " MODE #" + channel + " -i\r\n"));
		Utils::Send(client->first, std::string(":" + client->second->nickname + " MODE #" + channel + " -i\r\n"));
	}
}

void	Commands::parseArgs(std::vector<std::string> &args, std::map<int, t_clientInfo*>::iterator client, std::string channel)
{
	std::map<std::string, char>	flags = modeGetFlags(args[0]);
	args.erase(args.begin());
	std::map<std::string, char>::iterator it;
	(void)client;

	for (it = flags.begin(); it != flags.end(); it++)
	{
		if (it->first == "o")
			Commands::modeOperator(client, it->second, channel, args);
		else if (it->first == "k")
			Commands::modeKeypass(client, it->second, channel, args);
		else if (it->first == "l")
			Commands::modeUserLimit(client, it->second, channel, args);
		else if (it->first == "t")
			Commands::modeTopicAccess(client, it->second, channel, args);
		else if (it->first == "i")
			Commands::modeInviteOnly(client, it->second, channel, args);
		else
			Utils::Send(client->first, std::string(":127.0.0.1 472 " + client->second->nickname + " " + it->first + " :is unknown mode char to me\r\n"));	
	}
}

void	Commands::returnModeInfo(std::string channel, std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "Sending channel info back" << std::endl;
	std::string	curMods = "+";
	std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);

	if (chan->second->password.size())
	{
		curMods += "k ";
		curMods += chan->second->password;
	}
	if (chan->second->isInviteOnly)
		curMods += "i";
	if (chan->second->isTopicOPOnly)
		curMods += "t";
	if (chan->second->limit)
	{
		std::stringstream	funkyItoa;
		funkyItoa << chan->second->limit;
		curMods += "l ";
		curMods += funkyItoa.str();
	}
	Utils::Send(client->first, std::string(":127.0.0.1 324 " + client->second->nickname + " #" + channel + " " + curMods + "\r\n"));
}

void Commands::mode(std::map<int, t_clientInfo*>::iterator client)
{
	std::vector<std::string>				args = client->second->cmdtoken.args;
	std::string	channel = client->second->cmdtoken.target.substr(1);

	if (Channels::isChannelReal(channel))
	{
		if (!Channels::isUserOp(channel, client->second->nickname))
		{
			Utils::Send(client->first, std::string(":127.0.0.1 482 " + client->second->nickname + " :You are not channel operator\r\n"));	
			return ;
		}
		if (!args.size())
		{
			returnModeInfo(channel, client);
			return ;
		}
		parseArgs(args, client, channel);
	}
	else
		Utils::Send(client->first, std::string(":127.0.0.1 403 " + client->second->nickname + " :Channel does not exist\r\n"));	
}
