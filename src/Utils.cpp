/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 07:57:56 by mbirou            #+#    #+#             */
/*   Updated: 2025/03/11 08:05:38 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Includes.hpp>

void	Utils::Send(const int &fd, const std::string &msg)
{
	send(fd, msg.c_str(), msg.length(), MSG_DONTWAIT);
}
