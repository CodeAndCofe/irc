/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 17:45:17 by amandour          #+#    #+#             */
/*   Updated: 2026/01/30 17:24:54 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/Server.hpp"

void Server::invite(std::string data, Client client)
{
	std::vector<std::string> command = Server::split(data, ' ');

	if (command.size() < 3)
	{
		Server::send_msg(ERR_NEEDMOREPARAMS(data), client.getFd());
		return ;
	}
	if (command[2][0] != '#')
	{
		Server::send_msg(ERR_BADCHANMASK(command[2]), client.getFd());
		return ;
	}
	Client *newMember = getClient(command[1]);
	if (!newMember)
	{
		Server::send_msg(ERR_NOSUCHNICK(command[1]), client.getFd());
		return;
	}
	Channel *channel = getChannel(command[2]);
	if (!channel)
	{
		Server::send_msg(ERR_NOSUCHCHANNEL(command[2]), client.getFd());
		return ;
	}
	if (!channel->memberExist(client))
	{
		Server::send_msg(ERR_NOTONCHANNEL(client.getNickname(), command[2]), client.getFd());
		return ;
	}
	if (channel->memberExist(*newMember))
	{
		Server::send_msg(ERR_USERONCHANNEL(command[2], command[1]), client.getFd());
		return ;
	}
	if (channel->getInvOnlyMode() && !channel->isAdmine(client))
	{
		Server::send_msg(ERR_CHANOPRIVSNEEDED(command[2]), client.getFd());
		return ;
	}
	if (channel->getlimits() && channel->getMembers().size() + 1 > channel->getlimits())
	{
		Server::send_msg(ERR_CHANNELISFULL(client.getNickname(), command[1]), client.getFd());
		return ;
	}

	channel->inviteClient(*newMember);
	Server::send_msg(RPL_INVITING(client.getNickname(), newMember->getNickname(), channel->getName()), client.getFd());
}