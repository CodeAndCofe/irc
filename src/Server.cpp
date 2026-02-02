/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 16:03:06 by aferryat          #+#    #+#             */
/*   Updated: 2026/02/02 16:58:34 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

Server::Server(char *port, char *password)
{
    long    port_val;
    std::string str_port(port);
    std::string str_pass(password);

    for (size_t i = 0; i < str_port.length(); i++)
    {
        if (!std::isdigit(str_port[i]))
            throw Server::InvalidPortInput();
    }
    if (str_port.length() > 5)
        throw Server::InvalidPortInput();
    
    port_val  = std::atoi(str_port.c_str());
    if (port_val > 65535 || port_val < 0)
        throw   Server::InvalidPortInput();
    for (size_t i = 0; i < str_pass.length(); i++)
    {
        if (!std::isprint(str_pass[i]) || std::isspace(str_pass[i]))
            throw Server::InvalidPassowrdInput();
    }
    this->port = port_val;
    this->password = str_pass;
}

Server::Server()
{
    
}

Server::Server(Server &copy)
{
    this->port = copy.port;
	this->password = copy.password;
}

Server  &Server::operator=(Server &copy)
{
    (void) copy;
    return (*this);
}

Server::~Server()
{
    for (size_t i = 1; i < fds.size(); i++)
	{
		close(fds[i].fd);
	}
	close(this->serv);
}

// server functions
void	Server::create_socket()
{
    struct  sockaddr_in	server_address;
    int	fd;
    int res;

	fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw  "Error: socket fails";
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
		throw  "Error: fcntl fails";
	this->serv = fd;
    server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;
    int f = 1;
	res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,  &f, sizeof (sockaddr_in));
    if (res < 0)
        throw  "Error: setsockopt fails";
    if (bind(fd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0)
	{
		close(fd);
		throw  "Error: bind fails";
	}
}

int		Server::new_client(sockaddr_in client_address)
{
	int	            fd;
	Client	        new_client;
	struct	pollfd  newfds;
	socklen_t	    client_len = sizeof(client_address);

	fd = accept(this->serv, (struct sockaddr *) &client_address, &client_len);
	if (fd < 0)
        throw "Error: accept fails";
	if (fcntl(fd, F_SETFL, O_NONBLOCK))
        throw   "Error: fcntl fails";
	newfds.fd = fd;
	newfds.events = POLLIN;
	this->setfds(newfds);
	new_client.setFd(fd);
	this->setClient(new_client);
	return (0);
}

int Server::serverProcess(sockaddr_in &client_address)
{
    if (poll(&this->fds[0], fds.size(), -1) < 0)
		return (1);
    for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == this->serv && (fds[i].revents & POLLIN))
		{
			if (this->new_client(client_address) > 0)
				continue ;
		}
		if (fds[i].fd != this->serv && (fds[i].revents & POLLIN))
			this->clientHandler(this->clients[i - 1]);
		if (fds[i].fd != this->serv && (fds[i].revents & POLLHUP))
		{
            this->erase_client(i);
            i--;
		}
	}
    return (0);
}


//getters
std::string Server::get_password()
{
    return (this->password);
}

int Server::get_port()
{
    return (port);
}

int		Server::get_server_socket()
{
    return (this->serv);
}
//set data

void	Server::setClient(Client &client)
{
	this->clients.push_back(client);
}

void	Server::setfds(struct pollfd fds)
{
	this->fds.push_back(fds);
}
//  clear data

void	Server::erase_client(int i)
{
	close(fds[i].fd);
	this->clients.erase(clients.begin() + (i - 1));
    fds.erase(fds.begin() + i);
}

// errors
const char* Server::InvalidPortInput::what() const throw()
{
    return "Error: Invalid Port Value";
}

const char* Server::InvalidPassowrdInput::what() const throw()
{
    return "Error: Invalid Password Value";
}

//other uses:
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