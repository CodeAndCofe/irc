/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/18 14:57:07 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

int	client_acess(Client &t_client)
{
	std::string	my_buffer;
	std::string	cmd;
	if (!t_client.getHaspass())
	{
		cmd = "PASS";
		my_buffer = t_client.getBuffer();
		if (cmd.compare(my_buffer) != 0)
			return (-1);
		my_buffer.erase(0, cmd.length());
		std::cout << my_buffer << std::endl;
	}
	else if (!t_client.getHasNick())
	{
		cmd = "NICK";
		my_buffer = t_client.getBuffer();
		if (cmd.compare(my_buffer) != 0)
			return (-1);
		my_buffer.erase(0, cmd.length());
		std::cout << my_buffer << std::endl;
	}
	else if (!t_client.getHasUser())
	{
		cmd = "USER";
		my_buffer = t_client.getBuffer();
		if (cmd.compare(my_buffer) != 0)
			return (-1);
		my_buffer.erase(0, cmd.length());
		std::cout << my_buffer << std::endl;
	}
	return (1);
}

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
	std::cout << "Received: " << data << std::endl;
	t_client.setBuffer(data);
	if (client_acess(t_client) == -1)
		return (1);
	// CommandHandler(int fd, std::string &data, Client *client);
    return 0;
}