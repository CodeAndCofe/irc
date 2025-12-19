/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:24:45 by amandour          #+#    #+#             */
/*   Updated: 2025/12/18 22:32:48 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../headers/Channel.hpp"
#include "../headers/Client.hpp"
#include "../headers/Server.hpp"


void send_channel(std::string receiver, std::string msg, Client sender, Channel *channel)
{
	if (!channel)
	{
		Server::send_msg(ERR_CHANNELNOTFOUND(receiver), sender.getFd());
		return;
	}
	if (!channel->memberExist(sender))
	{
		Server::send_msg(ERR_CANNOTSENDTOCHAN(sender.getNickname(), channel->getName()), sender.getFd());
		return;
	}
	std::vector<Client> members = channel->getMembers();
	for(size_t i = 0; i < members.size(); i++)
	{
		if (sender.getFd() != members[i].getFd())
		{
			Server::send_msg(RPL_PRIVMSG(sender.getNickname() + "!" + sender.getUserName() + "@" + sender.getIpadd(), receiver, msg), members[i].getFd());
		}
	}
}

void send_user(std::string receiver, std::string msg, Client sender, Client *receiv)
{
	if (!receiv)
	{
		Server::send_msg(ERR_NOSUCHNICK(receiver), sender.getFd());
		return;
	}
	Server::send_msg(RPL_PRIVMSG(sender.getNickname() + "!" + sender.getUserName() + "@" + sender.getIpadd(), receiv->getNickname(), msg), receiv->getFd());
}

void Server::privmsg(std::string data, Client user)
{
	size_t found = data.find(':');
	std::vector<std::string> command = Server::split(data, ' ');
	
	if (command.size() < 3)
	{  
		Server::send_msg((ERR_MISSINGPARAMS(data)), user.getFd());
		return ;
	}
	
	std::string msg ;
	if (found == std::string::npos)
		msg = command[2];
	else
		msg = data.substr(found + 1, data.size());

	if (msg.empty())
	{
		Server::send_msg((ERR_NOMESSAGE()), user.getFd());
		return ;
	}

	std::vector<std::string> receiver = Server::split(command[1], ',');
	for (size_t i = 0; i < receiver.size(); i++)
	{
		if (receiver[i][0] == '#')
			send_channel(receiver[i], msg, user, getChannel(receiver[i]));
		else
			send_user(receiver[i], msg, user, getClient(receiver[i]));
	}
}
