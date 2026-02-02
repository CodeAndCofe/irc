/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:25:13 by amandour          #+#    #+#             */
/*   Updated: 2026/01/30 17:25:03 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/Server.hpp"

void Server::kick(std::string data, Client client)
{
	std::vector<std::string> command = Server::split(data, ' ');
    if (command.size() < 3)
	{  
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), client.getFd());
		return ;
	}
    size_t  found = data.find(':');
    if (command[1][0] != '#')
    {
        Server::send_msg(ERR_BADCHANMASK(command[1]), client.getFd());
		return ;
    }
    Channel *channel = getChannel(command[1]);
    if (!channel)
    {
        Server::send_msg(ERR_NOSUCHNICK(command[1]), client.getFd());
		return;
    }
    if (!channel->memberExist(client))
    {
        Server::send_msg(ERR_NOTONCHANNEL(client.getNickname(), command[1]), client.getFd());
		return ;
    }
    if (!channel->isAdmine(client))
    {
        Server::send_msg(ERR_CHANOPRIVSNEEDED(command[1]), client.getFd());
		return ;
    }
    std::vector<std::string> names = Server::split(command[2], ',');
    for (size_t i = 0; i < names.size(); i++)
    {
        Client *ban = getClient(names[i]);
        if (!ban || !channel->memberExist(*ban))
            Server::send_msg(ERR_USERNOTINCHANNEL(client.getNickname(), command[1]), client.getFd());
        else
        {
            channel->kickMember(*ban);
            if (found == std::string::npos && command.size() > 3)
                Channel::sendKickingMsg(client, *channel, *ban, command[3]);
            else if (found != std::string::npos && command[3].size() > 1)
                Channel::sendKickingMsg(client, *channel, *ban, data.substr(found + 1));
            else
                Channel::sendKickingMsg(client, *channel, *ban, "");
        }
    }
}