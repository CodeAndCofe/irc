/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:15:44 by amandour          #+#    #+#             */
/*   Updated: 2026/01/06 12:55:19 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/Server.hpp"
#include "../headers/RepErr.hpp"
#include <vector>
#include <cstddef>


void verifyKey(std::string pass, Client *user, Channel *channel, int fd)
{
    if (channel->memberExist(*user))
        return;
    if (pass == channel->getKey())
    {
        Server::send_msg(RPL_JOIN(user->getNickname(), channel->getName()), user->getFd());
        channel->addNewMember(*user);
        Channel::sendWelcomeMsg(*user, *channel);
        if (channel->getTopic().empty())
            Server::send_msg(RPL_NOTOPIC(user->getNickname(), channel->getName()), user->getFd());
        else
            Server::send_msg(RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()), user->getFd());
        Server::send_msg(RPL_NAMREPLY(user->getNickname(), channel->getName(), channel->nameReply()), user->getFd());
        Server::send_msg(RPL_ENDOFNAMES(user->getNickname(), channel->getName()), user->getFd());
    }
    else
        Server::send_msg(ERR_BADCHANNELKEY(user->getNickname(), channel->getName()), fd);
}

void Server::leaveAll(Client *user)
{
    for (size_t i = 0; i < this->_channels.size(); i++)
    {
        std::vector<Client>& members = _channels[i].getMembers();
        std::vector<Client>::iterator it = members.begin();
        while (it != members.end())
        {
            if (user->getFd() == it->getFd())
            {
                it = members.erase(it);
                break;
            }
            Server::send_msg(RPL_PART(user->getNickname(), _channels[i].getName(), ""), it->getFd());
            it++;
        }
        Server::send_msg(RPL_PART(user->getNickname(), _channels[i].getName(), ""), user->getFd());
    }
}

void Server::join(int fd, std::string data, Client *user)
{  
    std::vector<std::string> command = Server::split(data, ' ');

    if (command.size() < 2 || command[1].size() < 2)
    {
        Server::send_msg((ERR_NEEDMOREPARAMS(data)), user->getFd());
        return ;
    }
    std::vector<std::string> name = Server::split(command[1], ',');
    std::vector<std::string> pass(name.size());
    if (command.size() > 2)
        pass = Server::split(command[2], ',');
    for (size_t i = 0; i < name.size(); i++)
    {
        if (name[i][0] != '#')
        {
            Server::send_msg(ERR_BADCHANMASK(name[i]), user->getFd());
            return ;
        }
        if (name[i] == "#0")
        {
            this->leaveAll(user);
            continue ;
        }
        Channel *room = getChannel(name[i]);
        if (!room)
        {
            Channel room(name[i]);
            room.setAdmin(*user);
			room.setKey(pass[i]);
            room.addNewMember(*user);
            this->_channels.push_back(room);
            Server::send_msg(RPL_JOIN(user->getNickname(), room.getName()), user->getFd());
            Server::send_msg(RPL_NAMREPLY(user->getNickname(), room.getName(), room.nameReply()), user->getFd());
            Server::send_msg(RPL_ENDOFNAMES(user->getNickname(), name[i]), user->getFd());
        }
        else
        {
            if (room->getInvOnlyMode() && !room->isInvited(*user))
            {
                Server::send_msg(ERR_INVITEONLYCHAN(user->getNickname(), room->getName()), user->getFd());
                continue ;
            }
            if (room->getlimits() && room->getMembers().size() + 1 > room->getlimits())
            {
                Server::send_msg(ERR_CHANNELISFULL(user->getNickname(), room->getName()), user->getFd());
                continue ;
            }
            verifyKey(pass[i], user, room, fd);
        }
    }
}