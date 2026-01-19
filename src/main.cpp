/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:50:23 by aferryat          #+#    #+#             */
/*   Updated: 2026/01/19 16:29:49 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

int	flag = 0;

void	handler_signal(int	sig)
{
	if (sig == SIGINT)
		flag = 1;
}

bool	is_white_space(std::string s)
{
	for (size_t i = 0 ; s.length() > i; i++)
	{
		if (std::isspace((unsigned char) s[i]))
			return (true);
	}
	return (false);
}

bool	is_character(std::string port)
{
	for (size_t i = 0; port.length() > i; i++)
	{
		if (std::isalpha(port[i]))
			return (true);
	}
	return (false);
}

bool is_valid_input(char *pass, char *port)
{
	std::string	Port(port);
	std::string	Pass(pass);
	if (Pass.empty() || is_white_space(Pass))
		return (1);
	if (Port.empty() || is_white_space(Port) || is_character(Port))
		return (1);
	return (0);
}

int	main(int arc, char **arv)
{
	struct	sockaddr_in	server_address;
	struct	sockaddr_in	client_address;
	struct	pollfd		fds;
	int					port;
	int					socket_fd;

	if (arc != 3 || !arv[2][0] || !arv[1][0])
		return (std::cerr << "invalid executing: should be (./ft_irc \"port\" \"password\")" << std::endl, 1);
	if (is_valid_input(arv[2], arv[1]) == 1)
		return (std::cerr << "invalid executing: should be (./ft_irc \"port\" \"password\")" << std::endl, 1);
	port = std::atoi(arv[1]); // You should parse the user port !!
	Server	myServer(port, arv[2]);
	socket_fd = myServer.Create_Socket();
	if (socket_fd < 0)
	{
		return (std::cerr << "error while creating socket function faild" << std::endl, 1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int f = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,  &f, sizeof (sockaddr_in));
	if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0)
	{
		close(socket_fd);
		return (std::cerr << "bind function faild" << std::endl, 1);
	}
	if (listen(socket_fd, 500) < 0)
	{
		close(socket_fd);
		return (std::cerr << "listen function faild" << std::endl, 1);
	}
	fds.fd = socket_fd;
	fds.events = POLLIN;
	myServer.setfds(fds);
	std::cout << "---------Server Started---------" << std::endl;
	std::signal(SIGINT, handler_signal);
	while (true)
	{
		if (myServer.return_events(client_address) == 1)
			return (1);
		if (flag == 1)
			break ; }
}
