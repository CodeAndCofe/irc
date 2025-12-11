/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:18:32 by amandour          #+#    #+#             */
/*   Updated: 2025/12/10 18:09:17 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Channel.hpp"
#include "../headers/RepErr.hpp"
#include "../headers/Server.hpp"

Channel ::Channel() : name(""), invOnly(false), topicMode(false), limits(0){}
Channel ::Channel(std::string name) : name(name), invOnly(false), topicMode(false), limits(0){}

std::string Channel :: getKey()
{
	return (key);
}
std::string Channel :: getName()
{
	return (name);
}
std::vector<Client> Channel :: getAdmins()
{
	return (admins);
}

std::string Channel :: getTopic()
{
	return (topic);
}

std::vector<Client>& Channel::getMembers()
{
	return (members);
}

void Channel :: setKey(std ::string key)
{
	this->key = key;
}

void Channel :: setAdmin(Client client)
{
	admins.push_back(client);
}

void Channel::setTopic(std::string _topic)
{
	topic = _topic;
}

void Channel::addNewMember(Client client)
{
	this->members.push_back(client);
}


bool Channel::memberExist(Client client)
{
	for (size_t i = 0; i < members.size(); i++)
	{
		if (client.getFd() == members[i].getFd())
			return true;
	}
	return false;
}

bool Channel::isInvited(Client client)
{
	for (size_t i = 0; i < invited.size(); i++)
	{
		if (client.getFd() == invited[i].getFd())
			return true;
	}
	return false;
}
size_t   Channel::getlimits()
{
	return (limits);
}

std::string Channel::nameReply()
{
	std::string reply;
	for (size_t i = 0; i < members.size(); i++)
	{
		if (isAdmine(members[i]))
			reply += '@';
		reply += members[i].getNickname();
		if (i + 1 < members.size())
			reply += ' ';
	}
	return reply;
}

void Channel::sendWelcomeMsg(Client client, Channel channel)
{
    std::vector<Client>& members = channel.getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
        Server::send_msg(RPL_JOINMSG(client.getNickname() + '!' + client.getUserName(), client.getIpadd(), channel.getName()), members[i].getFd());
    }
}
bool Channel::getInvOnlyMode()
{
	return invOnly;
}
bool Channel::isAdmine(Client client)
{
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (client.getFd() == admins[i].getFd())
			return true;
	}
	return false;
}

void Channel::inviteClient(Client client)
{
	invited.push_back(client);
}