/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 13:08:37 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/04 16:54:41 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int	socketDesc;
	struct sockaddr_in server;
	socklen_t socketlen = sizeof(server);
	socketDesc = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (socketDesc == -1)
	{
		std::cout << "socketdesc fail: " << socketDesc << std::endl;
		exit(0);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(1234);
	if (bind(socketDesc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		std::cout << "bind fail" << std::endl;
		perror("error");
		exit(0);
	}
	if(listen(socketDesc, 10) < 0)
	{
		std::cout << "listen failed" << std::endl;
		close(socketDesc);
	}



	struct	epoll_event	pollevent, pollevents[10];
	int	pollfd = epoll_create1(0);
	if (pollfd < 0)
	{
		std::cout << "pollfd failed" << pollfd << std::endl;
		perror("error");
	}
	pollevent.events = EPOLLIN;
	pollevent.data.fd = pollfd;
	if (epoll_ctl(pollfd, EPOLL_CTL_ADD, socketDesc, &pollevent) < 0)
	{
		std::cout << "epoll_ctl failed" << pollfd << std::endl;
		perror("error");
	}


	int clientfd = 0;
	while (1)
	{
		if (pollfd < 0)
		{
			std::cout << "2. pollfd failed" << pollfd << std::endl;
			perror("error");
		}
		int	nfds = epoll_wait(pollfd, pollevents, 10, 0);
		if (nfds < 0)
		{
			std::cout << "nfds fail: " << nfds << std::endl;
			perror("error");
			break ;
		}
		for (int i = 0; i < nfds; ++i)
		{
			if (pollevents[i].data.fd == pollfd)
			{
				clientfd = accept(socketDesc, (struct sockaddr *)&server, &socketlen);
				pollevent.events = EPOLLIN;
				pollevent.data.fd = clientfd;
				if (epoll_ctl(pollfd, EPOLL_CTL_ADD, clientfd, &pollevent) < 0)
				{
					std::cout << "epoll_ctl failed" << pollfd << std::endl;
					perror("error");
				}
				std::cout << "new client" << std::endl;
			}
			else
			{
				char buffer[1024] = {0};
				int bread = recv(pollevents[i].data.fd, buffer, sizeof(buffer), MSG_DONTWAIT);
				if (bread <= 0)
					close(pollevents[i].data.fd);
				else
				{
					std::cout << "received: " << buffer << "|||" << std::endl;
					send(pollevents[i].data.fd, buffer, sizeof(buffer), MSG_DONTWAIT);
				}
			}
		}
	}
	close(socketDesc);
	close (pollfd);


}
