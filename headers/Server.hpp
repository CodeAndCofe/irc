/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:07:32 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/05 13:21:22 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <csignal>
#include <vector>

typedef struct	t_Client
{
	int	fd;
	std::string	nickname;
	std::string	username;
	std::string realname;
	std::string mssg;
	// std::string chanel;
}Client;

class	Server
{
	private:
		int	port;
		int	ser;
		
		std::string	password;
		std::vector<struct pollfd> fds;
		std::vector<Client> clients;
		
	public:
		int	Create_Socket();
		int	new_client(sockaddr_in client_address, int i);
		void	erase_client(int i);
		int	return_events(sockaddr_in client_address);
		void	setfds(struct pollfd fds);
		void	setClient(Client client);
		Server();
		Server(int port, int password);
		Server(Server &copy);
		Server &operator=(Server &copy);
		~Server(); 
		
};
int	client_message(Client &t_client);


#endif