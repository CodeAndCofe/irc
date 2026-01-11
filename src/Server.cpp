/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:31:53 by aferryat          #+#    #+#             */
/*   Updated: 2026/01/09 17:43:58 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"
#include "../headers/Channel.hpp"

Server::Server()
{
	
}

Server::Server(int port, char *password)
{
	std::string	my_pass(password);

	this->port = port;
	this->password = my_pass;
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
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << "BAD FILEDESCRIPTOR" << std::endl;
		return (-1);
	}
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
		std::cerr << "Error: " << this->fds[i].fd << std::endl;
		return (1);
	}
	newfds.fd = fd;
	if (fcntl(newfds.fd, F_SETFL, O_NONBLOCK))
	{
		std::cerr << "BAD FILEDESCRIPTOR" << std::endl;
		return (-1);
	}
	newfds.events = POLLIN;
	this->setfds(newfds);
	new_client.setFd(fd);
	new_client.create_ip(client_address);
	this->setClient(new_client);
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
		return (1);
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == this->ser && (fds[i].revents & POLLIN))
		{
			if (this->new_client(client_address, i) > 0)
				continue ;
		}
		if (fds[i].fd != this->ser && (fds[i].revents & POLLIN))
			client_message(this->clients[i - 1]);
		if (fds[i].fd != this->ser && (fds[i].revents & POLLHUP))
		{
			std::cout << "Client disconnected: " << fds[i].fd << std::endl;
            this->erase_client(i);
            i--;
		}
	}
	return (0);
}

void  Server::send_msg(std::string data, int fd)
{
    send(fd, data.c_str(), data.size(), MSG_DONTWAIT);
}

std::vector<std::string> Server::split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
        result.push_back(token);
    return result;
}

Channel *Server::getChannel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (name == _channels[i].getName())
			return &_channels[i];
	}
	return NULL;
}


Client *Server::getClient(std::string client)
{
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (std::strncmp(client.c_str(), clients[i].getNickname().c_str(), client.length()) == 0)
			return &clients[i];
	}
	return NULL;
}
void Server::CommandHandler(int fd, std::string &data, Client *client)
{
	std::vector<std::string>	command = Server::split(data, ' ');
	while (!data.empty() && (data.back() == '\r' || data.back() == '\n'))
        data.pop_back();
     if (!std::strncmp(command[0].c_str(), "JOIN", command[0].length()))
        join(fd, data, client);
	  else if (!std::strncmp(command[0].c_str(), "TOPIC", command[0].length()))
		topic(data, client);
    else if (!std::strncmp(command[0].c_str(), "INVITE", command[0].length()))
        invite(data, *client);
	  else if (!std::strncmp(command[0].c_str(), "KICK", command[0].length()))
		kick(data, *client);
	 else if (!std::strncmp(command[0].c_str(), "PRIVMSG", command[0].length()))
        privmsg(data, *client);
	else if (!std::strncmp(command[0].c_str(), "MODE", command[0].length()))
		mode(data, *client);
    else
    	Server::send_msg(ERR_UNKNOWNCOMMAND(client->getNickname(), data), client->getFd());
}

std::string	Server::getPassword()
{
	return (this->password);		
}