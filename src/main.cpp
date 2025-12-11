/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:09:01 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/10 17:23:24 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

int	flag = 0;

void	handler_signal(int	sig)
{
	if (sig == SIGINT)
		flag = 1;
}

int	main(int arc, char **arv)
{
	struct	sockaddr_in	server_address;
	struct	sockaddr_in	client_address;
	struct	pollfd		fds;
	int					port;
	int					password = 0;
	int					socket_fd;
	if (arc != 3)
		return (std::cerr << "invalid executing: should be (./ft_irc \"port\" \"password\")" << std::endl, 1);
	port = std::atoi(arv[1]);
	Server	myServer(port, password);
	socket_fd = myServer.Create_Socket();
	if (socket_fd < 0)
	{
		return (std::cerr << "error while creating socket function faild" << std::endl, 1);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

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
	fds.events = POLLIN;//connection
	myServer.setfds(fds);
	std::cout << "---------Server Started---------" << std::endl;
	std::signal(SIGINT, handler_signal);
	while (true)
	{
		if (myServer.return_events(client_address) == 1)
			return (1);
		if (flag == 1)
			break ;
	}
}
