/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:41:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/03/17 13:26:30 by mbatty           ###   ########.fr       */
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
		std::cout << modeChar.substr(i, 1) << std::endl;
		if (mod.find(modeChar.substr(i, 1)) == mod.end())
			mod.insert(std::make_pair(modeChar.substr(i, 1), curMod));
		else
			std::cout << "double detected" << std::endl;
	}
	return (mod);
}

/*

	MODE with no parameters (MODE #CHAN_NAME)
	|
	->	returns https://modern.ircdocs.horse/#rplchannelmodeis-324
	
	
	*/

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
	if (mod == '+')
	{
		std::map<std::string, t_channelInfo*>::const_iterator chan = Channels::find(channel);
		chan->second->isTopicOPOnly = true;
		std::cout << "Adding topic limit: " << " to channel " << channel << std::endl;
		args.erase(args.begin());
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
		{
			std::cout << "Operator thingy" << std::endl;
			Commands::modeOperator(it->second, channel, args);
		}
		if (it->first == "k")
		{
			std::cout << "Keypass thingy" << std::endl;
			Commands::modeKeypass(it->second, channel, args);
		}
		if (it->first == "l")
		{
			std::cout << "Limit thingy" << std::endl;
			Commands::modeUserLimit(it->second, channel, args);
		}
		if (it->first == "t")
		{
			std::cout << "Topic thingy" << std::endl;
			Commands::modeTopicAccess(it->second, channel, args);
		}
		if (it->first == "i")
		{
			std::cout << "Invite thingy" << std::endl;
			Commands::modeTopicAccess(it->second, channel, args);
		}
	}
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		std::cout << "Remaining args: " << it.base() << std::endl;
		// for (unsigned long int i = 0; i < flags.size(); i++)
	// {
	// 	if (flags[i][1] == 'i')
	// 		std::cout << "Invite only" << std::endl;
	// 		//modeInviteOnly();
	// 	if (flags[i][1] == 't')
	// 		std::cout << "Topic thingy" << std::endl;
	// 		//modeSetTopicCommand();
	// 	if (flags[i][1] == 'k')
	// 		std::cout << "Channel password" << std::endl;
	// 		//modeSetChannelKey();
	// 	if (flags[i][1] == 'o')
	// 		std::cout << "Set operator" << std::endl;
	// 		//modeSetOperator();
	// 	if (flags[i][1] == 'l')
	// 		std::cout << "Channel limit" << std::endl;
	// 		//modeChanLimit();
	// }
}

static void	returnModeInfo(std::string channel, std::map<int, t_clientInfo*>::iterator client)
{
	std::cout << "Sending channel info back" << std::endl;
	Utils::Send(client->first, std::string(":127.0.0.1 324 " + client->second->nickname + " " + "#" + channel + " caca\r\n"));	
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
