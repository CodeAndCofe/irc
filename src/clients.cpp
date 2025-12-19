/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/19 20:56:54 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

int	Server::isNickNameExist(std::string mybuffer)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].getNickname() == mybuffer)
			return (std::cout << "USER EXIST TWICE" << std::endl, 1);
	}
	return  (-1);
}


int	Server::client_acess(Client &t_client)
{
	std::string	my_buffer;
	std::string	cmd;
	if (t_client.getHaspass() == false)
	{
		cmd = "PASS";
		my_buffer = t_client.getBuffer();
		if (!std::strncmp(my_buffer.c_str(), "PASS", 4))
			return (-1);
		my_buffer.erase(0, cmd.length() + 1);
		if (std::strncmp(my_buffer.c_str(), this->password.c_str(), this->password.length()) == 0)
		{
			t_client.setHaspass(true);
			std::cout << "sucess passowrd" << std::endl;
		}
		return (-1);
	}
	if (t_client.getHasNick() == false)
	{
		cmd = "NICK";
		my_buffer = t_client.getBuffer();
		if (std::strncmp(my_buffer.c_str(), "NICK", 4) != 0)
			return (-1);
		my_buffer.erase(0, cmd.length() + 1);
		my_buffer = my_buffer.erase(my_buffer.length() - 1, my_buffer.length());
		if (isNickNameExist(my_buffer) == 1)
			return (-1);
		t_client.setNickname(my_buffer);
		t_client.setHasNick(true);
		std::cout << "sucess NICK NAME" << std::endl;
		return (-1);
	}
	if (t_client.getHasUser() == false)
	{
		cmd = "USER";
		my_buffer = t_client.getBuffer();
		if (std::strncmp(my_buffer.c_str(), "USER", 4) != 0)
			return (-1);
		my_buffer.erase(0, cmd.length() + 1);
		t_client.setUsername(my_buffer.erase(my_buffer.length() - 1, my_buffer.length()));
		t_client.setHasUser(true);
		std::cout << "sucess USERNAME" << std::endl;
		return (-1);
	}
	return (1);
}

int	Server::client_message(Client &t_client, std::vector<Client> &clients)
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
	t_client.setBuffer(data);
	if (client_acess(t_client) == -1)
		return (1);
	CommandHandler(t_client.getFd(), data, &t_client);
    return 0;
}