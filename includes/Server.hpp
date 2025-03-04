/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:24:52 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/04 19:23:03 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>

typedef struct	s_clientInfo
{
	std::string	surname;
	std::string	nickname;
	int			fd;
}				t_clientInfo;

class Server
{
	public:
		static void	ft_IRC();

	private:
		Server();
		Server(const Server &src);
		Server	operator =(const Server &rhs);
		~Server();

		static std::map<std::string, t_clientInfo>	_clients;
};