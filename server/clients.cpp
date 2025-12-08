/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/08 16:06:24 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"


int	client_message(Client &t_client, std::vector<Client> &clients)
{
	if (!clients.data())
		return (0);
	char	message[2050];

	int	byte = recv(t_client.getFd(), message, sizeof(message) - 1, MSG_DONTWAIT);

	if (byte < 0)
		return (1);
	message[byte] = '\0';
	std::cout << message << std::endl;
	return (0);
}