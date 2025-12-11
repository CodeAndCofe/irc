/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/11 13:23:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"


int	client_message(Client &t_client, std::vector<Client> &clients)
{
	char buffer[1024];
	int		bytes = 1;
	std::string data;
	
    if (clients.empty())
		return 0;
	buffer[0] = 4;
	while (bytes != 0)
    {
        bytes = recv(t_client.getFd(), buffer, sizeof(buffer), MSG_DONTWAIT);
        if (bytes < 0)
			return (1);
		data.append(buffer, bytes);
		if (!std::isprint(data.back()))
			bytes = 0;
    }

    if (!data.empty())
        std::cout << "Received: " << data << std::endl;
    return 0;
}