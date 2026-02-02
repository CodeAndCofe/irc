/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 13:46:30 by aferryat          #+#    #+#             */
/*   Updated: 2026/02/02 17:06:26 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/Channel.hpp"

void    Server::regester_user(Client &t_client)
{
    if (t_client.getHaspass() == true
        && t_client.getHasNick() == true
        && t_client.getHasUser() == true)
    {
        t_client.setRegesterd(true);
    }
}

bool    Server::isNickNameExist(std::string mybuffer)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (mybuffer == this->clients[i].getNickname())
			return (true);
	}
	return  (false);
}


void    Server::password_client(Client &t_client, std::string &pass)
{
	if (t_client.getHaspass() == false)
	{
		if (pass == this->password)
		{	
			t_client.setHaspass(true);
			return ;
		}
		send_msg(ERR_PASSWDMISMATCH(pass), t_client.getFd());
	}
	return ;
}

void	Server::regester_userName(Client &t_client, std::string &user)
{
	if (t_client.getHasUser() == false)
	{
		t_client.setUsername(user);
		t_client.setHasUser(true);
		return ;
	}
	return ;
}

void    Server::regester_nickName(Client &t_client, std::string &nick)
{
	if (t_client.getHasNick() == false)
	{
		if (isNickNameExist(nick))
		{
            Server::send_msg(ERR_NICKNAMEINUSE(nick), t_client.getFd());
            return;
        }
		t_client.setNickname(nick);
		t_client.setHasNick(true);
	}
	return ;
}


void	Server::authentication(Client &t_client)
{
    std::vector<std::string> command = Server::split(t_client.getBuffer(), ' ');

    if (command.empty())
        return;
    if (command[0] != "USER" && command[0] != "NICK" && command[0] != "PASS")
    {
        send_msg(ERR_NOTREGISTERED(t_client.getBuffer()), t_client.getFd());
        return ;
    }
    else if ((command[0] == "USER" && command.size() < 5) || command.size() < 2)
    {
        Server::send_msg(ERR_NEEDMOREPARAMS (t_client.getBuffer()), t_client.getFd());
		return ;
    }
    if (command[0] == "PASS")
        password_client(t_client, command[1]);
    else if (command[0] == "USER")
        regester_userName(t_client, command[1]);
    else if (command[0] == "NICK")
        regester_nickName(t_client, command[1]);
    return ;
}


void    Server::clientHandler(Client &t_client)
{
    char        buffer[500];
    int         bytes;
    std::string data;

    while (1)
    {
        signal(SIGPIPE, SIG_IGN);
        bytes = recv(t_client.getFd(), buffer, 499, MSG_DONTWAIT);
        if (t_client.getBuffer().empty() && bytes <= 0)
        {
            std::cout << "client Disconnected" << std::endl;
            return ;
        }
        else if (!t_client.getBuffer().empty() && bytes <= 0)
            return ;
        buffer[bytes] = '\0';
        t_client.setBuffer(buffer);
        if (t_client.getBuffer()[t_client.getBuffer().length() - 2] == '\r'
        && t_client.getBuffer()[t_client.getBuffer().length() - 1] == '\n')
            break;
    }
    data = t_client.getBuffer();
	data.erase(data.length() - 2);
    t_client.empty_buffer();
    t_client.setBuffer(data);
    if (!t_client.getRegestred())
    {
        authentication(t_client);
        regester_user(t_client);
        t_client.empty_buffer();
        return ;
    }
    CommandHandler(t_client.getFd(), t_client.getBuffer(), &t_client);
    t_client.empty_buffer();
    return ;
}

void Server::CommandHandler(int fd, std::string &data, Client *client)
{
	std::vector<std::string>	command = Server::split(data, ' ');

    if (command[0] == "JOIN")
        join(fd, data, client);
	  else if (command[0] == "TOPIC")
		topic(data, client);
    else if (command[0] == "INVITE")
        invite(data, *client);
	  else if (command[0] == "KICK")
		kick(data, *client);
	 else if (command[0] == "PRIVMSG")
        privmsg(data, *client);
	else if (command[0] == "MODE")
		mode(data, *client);
    else
    	Server::send_msg(ERR_UNKNOWNCOMMAND(client->getNickname(), data), client->getFd());
}