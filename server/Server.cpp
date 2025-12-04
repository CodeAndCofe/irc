/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:31:53 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/04 18:20:25 by aferryat         ###   ########.fr       */
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
	for (size_t i = 1; i < fds.size(); i++)
	{
		close(fds[i].fd);
	}
	close(this->ser);
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


void	Server::setClient(Client client)
{
	this->clients.push_back(client);
}

int		Server::return_events(sockaddr_in client_address)
{
	int	fd;
	Client			new_client;
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
			new_client.fd = fd;
			this->setClient(new_client);
			std::cout << "New client connected: " << fd << std::endl;
		}
		if (fds[i].fd != this->ser && (fds[i].revents & POLLIN))
		{
			if (client_message(this->clients[i - 1]) < 0)
			{
				std::cout << "Client disconnected: " << fds[i].fd << std::endl;
                close(fds[i].fd);
				this->clients.erase(clients.begin() + (i - 1));
                fds.erase(fds.begin() + i);
                i--;
			}
		}
	}
	return (0);
}