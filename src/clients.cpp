/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/11 15:01:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

int	command_wrong(std::string buffer, std::string s)
{
	if (s.compare(buffer) != 0)
		return (NULL)
	buffer.erase(0, s.length());
	return (buffer);
}

int	client_acess(Client &t_client, std::string password)
{
	if (!t_client.getHaspass())
	{
		std::string pass = command_wrong(t_client.getBuffer(), "PASS")
		if (pass == NULL || pass != password)
			return (-1);
	}
	return (1);
}


// int	client_nick(Client &t_client, std::string password)
// {
// 	if (!t_client.getHaspass())
// 	{
// 		std::string pass = command_wrong(t_client.getBuffer(), "PASS")
// 		if (pass == NULL || pass != password)
// 			return (-1);
// 	}
// 	return (1);
// }

int	client_message(Client &t_client, std::vector<Client> &clients, std::string password)
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
	std::cout << "Received: " << data << std::endl;
	t_client.setBuffer(data);
	if (client_acess(t_client, password) == -1)
		return (1);
	// CommandHandler(int fd, std::string &data, Client *client);
    return 0;
}