/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/04 17:17:06 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"


int	client_message(struct pollfd &fds)
{
	char	message[2050];

	int	byte = recv(fds.fd, message, sizeof(message) - 1, MSG_DONTWAIT);

	if (byte < 0)
		return (1);
	message[byte] = '\0';
	std::cout << message << std::endl;
	return (0);
}