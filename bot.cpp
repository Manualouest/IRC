/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 10:28:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/03/29 11:55:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Includes.hpp"

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 6969

bool	isRunning = false;
int					client_fd = 0;

void	stop(int sig)
{
	(void)sig;
	std::cout << "Stopping" << std::endl;
    send(client_fd, "QUIT\r\n", strlen("QUIT\r\n"), MSG_DONTWAIT);
	close(client_fd);
	isRunning = !isRunning;
}

void	sendServer(int client_fd, std::string str)
{
    send(client_fd, str.c_str(), str.size(), MSG_DONTWAIT);
}

std::string	readServer(int client_fd)
{
	int 		read_bytes = 1;
	char		buffer;
	std::string	res;
	
	while (read_bytes)
	{
		read_bytes = read(client_fd, &buffer, 1);
		if (!isRunning || buffer == '\n' || buffer == '\r')
			break ;
		res += buffer;
	}
	return (res);
}

int main(int ac, char **av)
{
    int					status = 0;
    struct sockaddr_in	serv_addr;

	if (ac != 3)
		return (1);

	isRunning = true;
	signal(SIGINT, stop);

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
	{
        printf("Socket creation error\n");
        return (1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(std::atoi(av[2]));
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (status < 0)
	{
        printf("Could not connect to server\n");
        return (1);
    }

	std::string	received;

    sendServer(client_fd, "PASS 123\r\n");

    sendServer(client_fd, "NICK Bot\r\n");

    sendServer(client_fd, "USER BotName 0 * :BotName2\r\n");
	
	sendServer(client_fd, "JOIN #botchannel\r\n");

	while (isRunning)
	{
		sendServer(client_fd, "PRIVMSG #botchannel :coucou caca\r\n");
		received = readServer(client_fd);
		if (received.size())
		{
			std::cout << received << std::endl;
		}
		received.clear();
	}

    return 0;
}
