/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 10:28:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/04/01 12:40:44 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "includes/Includes.hpp"

#include <Includes.hpp>

bool				isRunning = false;
int					client_fd = 0;

void	sendServer(const std::string &str)
{
	send(client_fd, str.c_str(), str.size(), MSG_DONTWAIT);
}

void	stop(int sig)
{
	(void)sig;
	PRINT "Stopping" CENDL;
	sendServer("QUIT :bot short circuited\r\n");
	close(client_fd);
	isRunning = !isRunning;
}

std::string	readServer(int client_fd)
{
	int 		read_bytes = 1;
	char		buffer;
	std::string	res;
	
	while (read_bytes)
	{
		read_bytes = recv(client_fd, &buffer, 1, MSG_WAITALL);
		if (read_bytes == 0 || !isRunning || (buffer == '\n' && res[res.length() - 1] == '\r'))
			break ;
		res += buffer;
	}
	return (res);
}

int	innitConnection(const std::string &port)
{
	struct sockaddr_in	serv_addr;

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
	{
		PRINT RED BOLD "Socket error: " AND strerror(errno) CENDL;
		return (0);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(std::atoi(port.c_str()));
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		PRINT RED BOLD "Connect error: " AND strerror(errno) CENDL;
		stop(0);
		return (0);
	}
	return (1);
}

int	connectToServ(const std::string &pass)
{
	sendServer("PASS " + pass + "\r\n");
	sendServer("NICK Bot\r\n");
	sendServer("USER BotName 0 * :BotName2\r\n");
	
	std::string	received = readServer(client_fd);
	if (received.find("001") == std::string::npos)
	{
		PRINT RED BOLD "Error when loggin in server" CENDL;
		stop(0);
		return (0);
	}
	return (1);
}

bool isSpace(char a)
{
	if (a == '!' || a == '"' || a == '#' || a == '$' || a == '%' || a == '&' || a == '\''
		|| a == '(' || a == ')' || a == '*' || a == '+' || a == ',' || a == '-' || a == '.'
		|| a == '/' || a == ':' || a == ';' || a == '<' || a == '=' || a == '>' || a == '?'
		|| a == '@' || a == '[' || a == '\\' || a == ']' || a == '^' || a == '_' || a == '`'
		|| a == '{' || a == '|' || a == '}' || a == '~')
		return (true);
	return (false);
}

int main(int ac, char **av)
{
	std::string	msgRecv;
	std::string	target;
	
	isRunning = true;
	signal(SIGINT, stop);

	if (ac != 3)
		PRINT RED BOLD "usage: " AND av[0] AND " <erver port> <server password>" CENDL;
	if (!innitConnection(av[1]) || !connectToServ(av[2]))
		return (-1);
	PRINT GRN BOLD "Connected to server." CENDL;

	while (isRunning)
	{
		msgRecv = readServer(client_fd);
		if (!msgRecv.size() || !isRunning)
			continue;
		// PRINT PRP AND msgRecv CENDL;
		if (!std::strncmp("You have been invited to #", msgRecv.c_str(), 26))
			sendServer("JOIN #" + msgRecv.substr(26, msgRecv.find(" by ", 26) - 26) + "\r\n");
		else if (msgRecv.find(" PRIVMSG ") != std::string::npos)
		{
			std::string tpMsg = msgRecv;
			std::string::iterator clear = std::remove_if(tpMsg.begin(), tpMsg.end(), isSpace);
			tpMsg.erase(clear, tpMsg.end());
			PRINT tpMsg CENDL;
			PRINT "saw msg" CENDL;
			PRINT tpMsg AND "\n, " AND (std::strcmp("quoi\r", tpMsg.c_str())) CENDL;
			if (tpMsg.find("quoi\r") != std::string::npos)
			{
				if (rand() % 2)
					sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":feur\r\n");
				else
					sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":coubeh\r\n");
			}
			if (tpMsg.find("ski\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":bidi\r\n");
			if (tpMsg.find("ping\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":pong\r\n");
			if (tpMsg.find("oui\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":stiti\r\n");
			if (tpMsg.find("cristaline\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":STALINE\r\n");
			if (tpMsg.find("ouais\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":stern\r\n");
			if (tpMsg.find("a\r") != std::string::npos)
				sendServer(msgRecv.substr(msgRecv.find("PRIVMSG "), msgRecv.find(":", msgRecv.find("PRIVMSG ")) - msgRecv.find("PRIVMSG ")) + ":pagnan\r\n");
		}
	}
	return 0;
}
