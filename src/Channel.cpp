/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:18:32 by amandour          #+#    #+#             */
/*   Updated: 2026/01/04 19:48:02 by aferryat         ###   ########.fr       */
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
	std::string	mssg;
	invited.push_back(client);
	// this->addNewMember(client);
	mssg = "You have been INVATED channel " + this->getName() + "\n";
	send(client.getFd(), mssg.c_str(), mssg.length(), MSG_DONTWAIT);
}

void Channel::kickMember(Client client)
{
	std::vector<Client>::iterator it = members.begin();
	while (it != members.end())
	{
		if (client.getFd() == it->getFd())
		{
			this->members.erase(it);
			return ;
		}
		it++;
	}
	if (isInvited(client))
	{
		std::vector<Client>::iterator it = invited.begin();
		while (it != invited.end())
		{
			if (client.getFd() == it->getFd())
			{
				this->invited.erase(it);
				return ;
			}
			it++;
		}
	}
}

void Channel::sendKickingMsg(Client sender, Channel channel, Client target, std::string comment)
{
    std::vector<Client>& members = channel.getMembers();
    for (size_t i = 0; i < members.size(); i++)
    {
		if (target.getFd() != members[i].getFd())
			Server::send_msg(RPL_KICKMSG(sender.getNickname() + "!~" + sender.getUserName() + "@" + sender.getIpadd(), channel.getName(), target.getNickname(), comment), members[i].getFd());
    }
	Server::send_msg(RPL_KICKDEFMSG(sender.getNickname() + "!~" + sender.getUserName() + "@" + sender.getIpadd(), channel.getName(), target.getNickname()), target.getFd());
}

bool   Channel::getTopicMode()
{
	return topicMode;
}

void Channel::removeAdmin(Client client)
{
	std::vector<Client>::iterator it = admins.begin();
	while (it != admins.end())
	{
		if (client.getFd() == it->getFd())
		{
			this->admins.erase(it);
			return ;
		}
		it++;
	}
}


void Channel::sendModeMsg(Client client, Channel channel, std::string mode, std::string param)
{
    std::vector<Client> members = channel.getMembers();
	if (!param.empty())
		mode = mode + ' ' + param;
    for (size_t i = 0; i < members.size(); i++)
    {
        Server::send_msg(RPL_CHANGEMODE(client.getNickname(), channel.getName(), mode), members[i].getFd());
    }
}
void Channel::setTopicMode(bool mode)
{
	topicMode = mode;
}
void Channel::setInviteMode(bool mode)
{
	invOnly = mode;
}

void Channel::setLimits(size_t number)
{
	limits = number;
}