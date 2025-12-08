/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:31:53 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/08 15:53:32 by aferryat         ###   ########.fr       */
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


void	Server::setClient(Client &client)
{
	this->clients.push_back(client);
}

int		Server::new_client(sockaddr_in client_address, int i)
{
	int	fd;
	Client	new_client;
	struct	pollfd newfds;
	socklen_t	client_len = sizeof(client_address);
	fd = accept(this->ser, (struct sockaddr *) &client_address, &client_len);
	if (fd < 0)
	{
		perror("accept failed");
		std::cout << "Error: " << this->fds[i].fd << std::endl;
		return (1);
	}
	newfds.fd = fd;
	newfds.events = POLLIN;
	this->setfds(newfds);
	new_client.setFd(fd);
	this->setClient(new_client);
	std::cout << "New client connected: " << fd << std::endl;
	return (0);
}

void	Server::erase_client(int i)
{
	close(fds[i].fd);
	this->clients.erase(clients.begin() + (i - 1));
    fds.erase(fds.begin() + i);
}

int		Server::return_events(sockaddr_in client_address)
{
	if (poll(this->fds.data(), fds.size(), -1) < 0)
	{
		std::cerr << "something went wrong" << std::endl;
		return (1);
	}
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == this->ser && (fds[i].revents & POLLIN))
		{
			if (this->new_client(client_address, i) > 0)
				continue ;
		}
		if (fds[i].fd != this->ser && (fds[i].revents & POLLIN))
		{
			if (client_message(this->clients[i - 1]) < 0)
			{
				std::cout << "Client disconnected: " << fds[i].fd << std::endl;
				this->erase_client(i);
                i--;
			}
		}
		if (fds[i].fd != this->ser && (fds[i].revents & POLLHUP))
		{
			std::cout << "Client disconnected: " << fds[i].fd << std::endl;
            this->erase_client(i);
            i--;
		}
	}
	return (0);
}
