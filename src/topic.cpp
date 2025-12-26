/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:38:05 by amandour          #+#    #+#             */
/*   Updated: 2025/12/25 18:02:10 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Server.hpp"

void Server::topic(std::string data, Client *client)
{
	std::vector<std::string> topic = Server::split(data, ' ');
	if (topic.size() < 2)
	{
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), client->getFd());
		return ;
	}
	if (topic[1][0] != '#')
	{
		Server::send_msg(ERR_BADCHANMASK(topic[1]), client->getFd());
		return ;
	}
	Channel *room = getChannel(topic[1]);
	if (!room)
	{
		Server::send_msg(ERR_NOSUCHCHANNEL(topic[1]), client->getFd());
		return;
	}

	if (!room->memberExist(*client))
	{
		Server::send_msg(ERR_NOTONCHANNEL(client->getNickname(), topic[1]), client->getFd());
		return ;
	}
	if (topic.size() < 3)
	{
		if (room->getTopic().empty())
			Server::send_msg(RPL_NOTOPIC(client->getNickname(), topic[1]), client->getFd());
		else
			Server::send_msg(RPL_TOPIC(client->getNickname(), topic[1], room->getTopic()), client->getFd());
	}
	else if (topic[2][0] == ':')
	{
		if (topic[2].size() == 1 && topic.size() == 3)
			room->setTopic("");
		else if (!room->getTopicMode() || (room->getTopicMode() && room->isAdmine(*client)))
			room->setTopic(data.substr(data.find(':') + 1));
		else
			Server::send_msg(ERR_CHANOPRIVSNEEDED(client->getNickname()), client->getFd());
	}
	else
	{
		if (!room->getTopicMode() || (room->getTopicMode() && room->isAdmine(*client)))
			room->setTopic(topic[2]);
		else
			Server::send_msg(ERR_CHANOPRIVSNEEDED(client->getNickname()), client->getFd());
	}
}