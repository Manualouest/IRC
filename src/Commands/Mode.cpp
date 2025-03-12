/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mewen.mewen@hotmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:41:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/03/12 13:53:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Commands.hpp>
#include <deque>

static std::deque<std::string> modeGetFlags(std::string modeChar)
{
	char					curMod = '+';
	std::deque<std::string>	mod;
	
	for (int i = 0; modeChar[i]; i++)
	{
		if (modeChar[i] == '\n' || modeChar[i] == ' ')
			break ;
		if (modeChar[i] == '-' || modeChar[i] == '+')
		{
			curMod = modeChar[i];
			continue ;
		}
		if (curMod == '-')
			mod.push_back("-" + modeChar.substr(i, 1));
		if (curMod == '+')
			mod.push_back("+" + modeChar.substr(i, 1));
	}
	return (mod);
}

#include <sstream>

/*

	MODE with no parameters (MODE #CHAN_NAME)
	|
	->	returns https://modern.ircdocs.horse/#rplchannelmodeis-324

	
*/

static void	parseArgs(std::deque<std::string> &args, std::map<int, t_clientInfo*>::iterator client)
{
	std::deque<std::string>	flags = modeGetFlags(args[0]);

	(void)client;

	for (long unsigned int i = 0; i < flags.size(); i++)
		std::cout << "Flags:" << flags[i] << std::endl;
	for (long unsigned int i = 0; i < args.size(); i++)
		std::cout << "Args:" << args[i] << std::endl;
	for (unsigned long int i = 0; i < flags.size(); i++)
	{
		if (flags[i][1] == 'i')
			std::cout << "Invite only" << std::endl;
			//modeInviteOnly();
		if (flags[i][1] == 't')
			std::cout << "Topic thingy" << std::endl;
			//modeSetTopicCommand();
		if (flags[i][1] == 'k')
			std::cout << "Channel password" << std::endl;
			//modeSetChannelKey();
		if (flags[i][1] == 'o')
			std::cout << "Set operator" << std::endl;
			//modeSetOperator();
		if (flags[i][1] == 'l')
			std::cout << "Channel limit" << std::endl;
			//modeChanLimit();
	}
}

void Commands::mode(std::map<int, t_clientInfo*>::iterator client)
{
	client->second->cmd = client->second->cmd.substr(client->second->cmd.find_first_of(' ') + 1);

	std::string				rawArgs = client->second->cmd;
	std::stringstream		rawStringStream(rawArgs);
	std::string				getlineRes;
	std::deque<std::string>	args;

	while (std::getline(rawStringStream, getlineRes, ' '))
	{
		getlineRes.substr(1, getlineRes.find_first_of("\n\r") - 1);
		args.push_back(getlineRes);
	}

	// DEBUG PRINT
	std::cout << "===MODE=== Raw args:" << rawArgs << std::endl;
	for (long unsigned int i = 0; i < args.size(); i++)
		std::cout << args[i] << std::endl;
	// DEBUG PRINT
	
	std::cout << "Channel name:" << args[0] << std::endl;
	// if (Channels::isChannelReal(args[0]))
	// {
		//if (doesUserHaveChanAccess(client))
		//	Utils::Send(client->first, std::string(":127.0.0.1 482 " + client->second->nickname + " :You do not have access\r\n"));	
		//if (args.size() == 1)
		//	returnModeInfo();
		std::cout << "do things here" << std::endl;
		args.pop_front();
		parseArgs(args, client);
	// }
	// else
	// {
		// std::cout << "Invalid chan name" << std::endl;
		// Utils::Send(client->first, std::string(":127.0.0.1 403 " + client->second->nickname + " :Channel does not exist\r\n"));	
	// }
}

// typedef	struct s_token
// {
// 	std::string					command;
// 	std::string					channel;
// 	std::vector<std::string>	args;
// }	t_token;