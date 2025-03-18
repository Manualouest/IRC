/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:41:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/03/17 14:28:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>
#include <deque>

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

void	Commands::modeOperator(char mod, std::string channel, std::vector<std::string> &args)
{
	if (!args.size())
	{
		std::cout << "No args given! (No error msg needed)" << std::endl;
		return ;
	}
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		if (Channels::isInChannel(chan->second->users, args[0]))
		{
			std::cout << "OP HERE" << std::endl;
		}
		std::cout << "Adding operator status: " << args[0] << " from channel " << channel << std::endl;
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		if (Channels::isInChannel(chan->second->users, args[0]))
		{
			std::cout << "DEOP HERE" << std::endl;
		}
		std::cout << "Removing operator status: " << args[0] << " from channel " << channel << std::endl;
		args.erase(args.begin());
	}
}

void	Commands::modeKeypass(char mod, std::string channel, std::vector<std::string> &args)
{
	if (mod == '+')
	{
		if (!args.size())
		{
			std::cout << "No args given! (No error msg needed)" << std::endl;
			return ;
		}
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->password = args[0];
		std::cout << "Adding channel password: " << args[0] << " from channel " << channel << std::endl;
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->password = "";
		std::cout << "Removing channel password: " << " from channel " << channel << std::endl;
	}
}

void	Commands::modeUserLimit(char mod, std::string channel, std::vector<std::string> &args)
{
	if (mod == '+')
	{
		if (!args.size() || std::atoi(args[0].c_str()) <= 0)
		{
			std::cout << "No args given! (No error msg needed)" << std::endl;
			return ;
		}
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->limit = std::atoi(args[0].c_str());
		std::cout << "Adding user limit: " << args[0] << " to channel " << channel << std::endl;
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->limit = 0;
		std::cout << "Removing user limit: " << " to channel " << channel << std::endl;
	}
}

void	Commands::modeTopicAccess(char mod, std::string channel, std::vector<std::string> &args)
{
	(void)args;
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isTopicOPOnly = true;
		std::cout << "Adding topic limit: " << " to channel " << channel << std::endl;
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isTopicOPOnly = false;
		std::cout << "Removing topic limit: " << " to channel " << channel << std::endl;
	}
}

void	Commands::modeInviteOnly(char mod, std::string channel, std::vector<std::string> &args)
{
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isInviteOnly = true;
		std::cout << "Adding invite only: " << " to channel " << channel << std::endl;
		args.erase(args.begin());
	}
	if (mod == '-')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isInviteOnly = false;
		std::cout << "Removing invite only: " << " to channel " << channel << std::endl;
	}
}

void	parseArgs(std::vector<std::string> &args, std::map<int, t_clientInfo*>::iterator client, std::string channel)
{
	std::map<std::string, char>	flags = modeGetFlags(args[0]);
	args.erase(args.begin());
	std::map<std::string, char>::iterator it;
	(void)client;

	for (it = flags.begin(); it != flags.end(); it++)
		std::cout << "Flags:" << it->second << it->first << std::endl;
	for (it = flags.begin(); it != flags.end(); it++)
	{
		if (it->first == "o")
			Commands::modeOperator(it->second, channel, args);
		if (it->first == "k")
			Commands::modeKeypass(it->second, channel, args);
		if (it->first == "l")
			Commands::modeUserLimit(it->second, channel, args);
		if (it->first == "t")
			Commands::modeTopicAccess(it->second, channel, args);
		if (it->first == "i")
			Commands::modeTopicAccess(it->second, channel, args);
	}
}

void	Commands::returnModeInfo(std::string channel, std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "Sending channel info back" << std::endl;
	std::string	curMods = "+";
	std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);

	if (chan->second->password.size())
		curMods += "k";
	if (chan->second->isInviteOnly)
		curMods += "i";
	if (chan->second->isTopicOPOnly)
		curMods += "t";
	if (chan->second->limit)
		curMods += "l";
	Utils::Send(client->first, std::string(":127.0.0.1 324 " + client->second->nickname + " #" + channel + " " + curMods + "\r\n"));
}

void Commands::mode(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(client->second->cmd.find_first_of(' ') + 1);

	std::vector<std::string>				args = client->second->cmdtoken.args;
	std::string	channel = client->second->cmdtoken.target.substr(1);

	if (Channels::isChannelReal(channel))
	{
		if (!client->second)
			Utils::Send(client->first, std::string(":127.0.0.1 482 " + client->second->nickname + " :You are not channel operator\r\n"));	
		if (!args.size())
		{
			returnModeInfo(channel, client);
			return ;
		}
		std::cout << "do things here" << std::endl;
		parseArgs(args, client, channel);
	}
	else
	{
		std::cout << "Invalid chan name" << std::endl;
		Utils::Send(client->first, std::string(":127.0.0.1 403 " + client->second->nickname + " :Channel does not exist\r\n"));	
	}
}
