
#include "../headers/Channel.hpp"
#include "../headers/Server.hpp"
#include <cctype>
#include <cstddef>

bool isDigit(std::string s)
{
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void Server::mode(std::string data, Client client)
{
	std::vector<std::string> command = Server::split(data, ' ');

	if (command.size() < 2)
	{
		Server::send_msg((ERR_NEEDMOREPARAMS(data)), client.getFd());
		return ;
	}
	if (command[1][0] == '#')
	{	
		Channel *room = getChannel(command[1]);
		if (!room)
		{
			Server::send_msg(ERR_NOSUCHCHANNEL(command[1]), client.getFd());
			return;
		}
		
		if (command.size() < 3 || command[2].size() < 2 || (command[2][0] != '+' && command[2][0] != '-'))
		{
			Server::send_msg(RPL_CHANNELMODEIS(client.getNickname(), room->getName(), "+/-", "i/t/k/o/l"), client.getFd());
			return;
		}
		if (!room->memberExist(client))
		{
			Server::send_msg(ERR_NOTONCHANNEL(client.getNickname(), room->getName()), client.getFd());
			return ;
		}
		if (!room->isAdmine(client))
		{
			Server::send_msg(ERR_CHANOPRIVSNEEDED(room->getName()), client.getFd());
			return ;
		}
		bool isPos = command[2][0] == '+';
		size_t curr = 3;
		for (size_t i = 1; i < command[2].size(); i++)
		{
			if (command[2][i] == '+')
				isPos = true;
			else if (command[2][i] == '-')
				isPos = false;
			else if (isPos)
			{
				if (command[2][i] == 'i')
				{
					room->setInviteMode(true);
					Channel::sendModeMsg(client, *room, "+i", "");
				}
				else if (command[2][i] == 't')
				{
					room->setTopicMode(true);
					Channel::sendModeMsg(client, *room, "+t", "");
				}
				else if (command[2][i] == 'k' && command.size() > curr)
				{
					room->setKey(command[curr]);
					Channel::sendModeMsg(client, *room, "+k", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'o' && command.size() > curr)
				{
					Client *member = getClient(command[curr]);
					if (!member || !room->memberExist(*member))
					{
						Server::send_msg(ERR_USERNOTINCHANNEL(client.getNickname(), room->getName()), client.getFd());
						curr++;
						continue ;
					}
					room->setAdmin(*member);
					Channel::sendModeMsg(client, *room, "+o", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'l' && command.size() > curr && isDigit(command[curr]))
				{
					room->setLimits(std::atol(command[curr].c_str()));
					Channel::sendModeMsg(client, *room, "+l", command[curr]);
					curr++;
				}
			}
			else if (!isPos)
			{
				if (command[2][i] == 'i')
				{
					room->setInviteMode(false);
					Channel::sendModeMsg(client, *room, "-i", "");
				}
				else if (command[2][i] == 't')
				{
					room->setTopicMode(false);
					Channel::sendModeMsg(client, *room, "-t", "");	
				}
				else if (command[2][i] == 'k')
				{
					room->setKey("");
					Channel::sendModeMsg(client, *room, "-k", "");	
				}
				else if (command[2][i] == 'o' && command.size() > curr)
				{
					Client *member = getClient(command[curr]);
					if (!member || !room->memberExist(*member))
					{
						Server::send_msg(ERR_USERNOTINCHANNEL(client.getNickname(), room->getName()), client.getFd());
						curr++;
						continue ;
					}
					room->removeAdmin(*member);
					Channel::sendModeMsg(client, *room, "-o", command[curr]);
					curr++;
				}
				else if (command[2][i] == 'l')
				{
					room->setLimits(0);
					Channel::sendModeMsg(client, *room, "-l", "");	
				}
			}
			else
				Server::send_msg(ERR_UMODEUNKNOWNFLAG(client.getNickname()), client.getFd());
		}
	}
}
