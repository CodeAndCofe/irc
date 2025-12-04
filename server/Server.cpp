/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:31:53 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/04 17:22:58 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server()
{
	
}
Server::Server(int port, int password)
{
	this->port = port;
	this->password = password;
}
Server::Server(Server &copy)
{
	this->port = copy.port;
	this->password = copy.password;
}

Server	&Server::operator=(Server &copy)
{
	this->port = copy.port;
	this->password = copy.password;
	return (*this);
}

Server::~Server()
{
	
}

int	Server::Create_Socket()
{
	int	fd;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	this->ser = fd;
	return (fd);
}

void	Server::setfds(struct pollfd fds)
{
	this->fds.push_back(fds);
}



int		Server::return_events(sockaddr_in client_address)
{
	int	fd;
	struct	pollfd newfds;
	poll(this->fds.data(), fds.size(), -1);
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == this->ser && (fds[i].revents & POLLIN))
		{
			
			socklen_t	client_len = sizeof(client_address);
			fd = accept(this->ser, (struct sockaddr *) &client_address, &client_len);
			if (fd < 0)
			{
				perror("accept failed");
				std::cout << "Error: " << fds[i].fd << std::endl;
				continue;
			}
			newfds.fd = fd;
			newfds.events = POLLIN;
			this->setfds(newfds);
			std::cout << "New client connected: " << fd << std::endl;
		}
		if (fds[i].fd != this->ser && (fds[i].revents & POLLIN))
		{
			if (client_message(fds[i]) < 0)
			{
				std::cout << "Client disconnected: " << fds[i].fd << std::endl;
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                i--;
			}
		}
	}
	return (0);
}