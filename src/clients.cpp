/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:30:07 by aferryat          #+#    #+#             */
/*   Updated: 2026/01/28 14:49:56 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

bool	is_regester(Client &t_client)
{
    if (t_client.getHaspass() == true
        && t_client.getHasNick() == true
        && t_client.getHasUser() == true)
    {
        t_client.setRegesterd(true);
        return (true);
    }
    return (false);
}

int	Server::isNickNameExist(std::string mybuffer)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (mybuffer == this->clients[i].getNickname())
			return (1);
	}
	return  (-1);
}

int	Server::password_client(Client &t_client, std::string cmd)
{
	std::vector<std::string> pass = Server::split(t_client.getBuffer(), ' ');
	if (pass.empty() || clean_compare(pass[0], cmd, cmd.length()) == false)
		return 0;
	if (t_client.getHaspass() == false)
	{
		if (pass.size() < 2)
		{
			Server::send_msg(ERR_NEEDMOREPARAMS (pass[0]), t_client.getFd());
			return (1);
		}
		if (clean_compare(pass[1], this->password, this->password.length()))
		{	
			t_client.setHaspass(true);
			return (1);
		}
		send_msg(ERR_PASSWDMISMATCH(pass[1]), t_client.getFd());
	}
	return (1);
}

int	Server::nick_name_regester(Client &t_client, std::string cmd)
{
	std::vector<std::string> nick = Server::split(t_client.getBuffer(), ' ');
	if (nick.empty() || clean_compare(nick[0], cmd, cmd.length()) == false)
		return (0);
	if (t_client.getHasNick() == false)
	{
		if (nick.size() < 2)
			return (1);
		if (isNickNameExist(nick[1]) == 1)
			return (Server::send_msg(ERR_NICKNAMEINUSE(nick[1]), t_client.getFd()), 1);
		t_client.setNickname(nick[1]);
		t_client.setHasNick(true);
		return (1);
	}
	return (1);
}

int	Server::user_name_regester(Client &t_client, std::string cmd)
{
	std::vector <std::string> user = Server::split(t_client.getBuffer(), ' ');
	if (user.empty() || clean_compare(user[0], cmd, cmd.length()) == false)
		return (1);
	if (user.size() < 5)
		return (send_msg(ERR_NEEDMOREPARAMS(user[1]), t_client.getFd()), 1);
	if (t_client.getHasUser() == false)
	{
		if (user.size() < 2)
			return (1);
		t_client.setUsername(user[1]);
		t_client.setHasUser(true);
		return (1);
	}
	return (1);
}

int	Server::client_acess(Client &t_client)
{
	if (!clean_compare(t_client.getBuffer().c_str(), "NICK", 4)
		&& !clean_compare(t_client.getBuffer().c_str(), "PASS", 4)
		&& !clean_compare(t_client.getBuffer().c_str(), "USER", 4))
			return (1);
	if (this->password_client(t_client, "PASS") == 1)
		return (-1);
	if (this->nick_name_regester(t_client, "NICK") == 1)
		return (-1);
	if (this->user_name_regester(t_client, "USER") == 1)
		return (-1);
	return (0);
}

int	Server::client_message(Client &t_client)
{
	char	buffer[2];
	int		bytes = 1;
	std::string data(t_client.getBuffer());

	t_client.empty_buffer();
	buffer[0] = 4;
	while (bytes != 0)
    {
		signal(SIGPIPE, SIG_IGN);
		bytes = recv(t_client.getFd(), buffer, 1, MSG_DONTWAIT);
		if (bytes <= 0)
		{
			t_client.setBuffer(data);
			return (0);
		}
		buffer[bytes] = '\0';
		data.append(buffer, bytes);
		if (data[data.size() - 1]== '\n')
			bytes = 0;
    }
	if (t_client.getBuffer().length() > 1024)
	{
		t_client.empty_buffer();
		return (1);
	}
	while (data[data.length() - 1] ==  '\r' || data[data.length() - 1] == '\n')
		data.erase(data.length() - 1);
	if (data.empty())
		return (0);
	t_client.setBuffer(data);
	if (is_regester(t_client) ||  t_client.getRegestred())
    {
        CommandHandler(t_client.getFd(), t_client.getBuffer(), &t_client);
		t_client.empty_buffer();
        return (0);
    }
	else if (client_acess(t_client) == -1)
	{
		if (is_regester(t_client))
			std::cout << "new client regestred" << std::endl;
		t_client.empty_buffer();
		return (0);
	}
	else
	{	
		send_msg(ERR_NOTREGISTERED(t_client.getBuffer()), t_client.getFd());
		t_client.empty_buffer();
	}
    return (0);
}
