/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/25 17:44:25 by amandour         ###   ########.fr       */
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

int	Server::password_client(Client &t_client, std::string cmd)
{
	std::vector<std::string> pass = Server::split(t_client.getBuffer(), ' ');
	if (pass.empty() || pass[0] != cmd)
		return 0;
	if (t_client.getHaspass() == false)
	{
		if (pass.size() < 2)
		{
			Server::send_msg(ERR_NEEDMOREPARAMS (pass[0]), t_client.getFd());
			return (1);
		}
		if (std::strncmp(pass[1].c_str(), this->password.c_str(), this->password.length()) == 0)
		{
			std::cout << "password has regestred" << std::endl;
			t_client.setHaspass(true);
			return (1);
		}
	}
	return (1);
}

int	Server::nick_name_regester(Client &t_client, std::string cmd)
{
	std::vector<std::string> nick = Server::split(t_client.getBuffer(), ' ');
	if (nick.empty() || nick[0] != cmd)
		return (0);
	if (t_client.getHasNick() == false)
	{
		if (nick.size() < 2)
			return (1);
		if (isNickNameExist(nick[1]) == 1)
			return (Server::send_msg(ERR_NICKNAMEINUSE(nick[1]), t_client.getFd()), 1);
		t_client.setNickname(nick[1]);
		t_client.setHasNick(true);
		std::cout << "NICK has regestred" << ":" << nick[1] << std::endl;
		return (1);
	}
	return (1);
}

int	Server::user_name_regester(Client &t_client, std::string cmd)
{
	std::vector <std::string> user = Server::split(t_client.getBuffer(), ' ');
	if (user.empty() || user[0] != cmd)
		return (0);
	if (t_client.getHasUser() == false)
	{
		if (user.size() < 2)
			return (1);
		t_client.setUsername(user[1]);
		t_client.setHasUser(true);
		std::cout << "USER has regestred" << ":" <<user[1] << std::endl;
		return (0);
	}
	return (1);
}


int	Server::client_acess(Client &t_client)
{
	if (!std::strncmp(t_client.getBuffer().c_str(), "PASS", 4)
		&& !std::strncmp(t_client.getBuffer().c_str(), "NICK", 4)
		&& !std::strncmp(t_client.getBuffer().c_str(), "USER", 4))
			return (send_msg(ERR_NOTREGISTERED, t_client.getFd()), 1);
	std::cout << t_client.getBuffer() << std::endl;
	if (this->password_client(t_client, "PASS") == 1)
		return (-1);
	if (this->nick_name_regester(t_client, "NICK") == 1)
		return (-1);
	if (this->user_name_regester(t_client, "USER") == 1)
		return (-1);
	if (t_client.getHaspass() == true
		&& t_client.getHasNick() == true
		&& t_client.getHasUser() == true)
	{
		t_client.setRegesterd(true);
		std::cout << "regestred fine" << std::endl;
	}
	return (1);
}

int	Server::client_message(Client &t_client)
{
	char buffer[1024];
	int		bytes = 1;
	std::string data;

	buffer[0] = 4;
	while (bytes != 0)
    {
		bytes = recv(t_client.getFd(), buffer, sizeof(buffer), MSG_DONTWAIT);
        if (bytes < 0)
			return (-1);
		data.append(buffer, bytes);
		if (!std::isprint(data.back()))
			bytes = 0;
    }
	data.erase(data.size(), -1);
	t_client.setBuffer(data);
	if (t_client.getRegestred() == false)
		if (client_acess(t_client) == -1)
			return (1);
	CommandHandler(t_client.getFd(), data, &t_client);
    return 0;
}