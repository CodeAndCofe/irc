/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:07:32 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/04 17:17:13 by aferryat         ###   ########.fr       */
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

class	Server
{
	private:
		int	port;
		int	ser;
		std::string	password;
		static bool Ssignal;
		std::vector<struct pollfd> fds;
		
		
	public:
		int	Create_Socket();
		int	return_events(sockaddr_in client_address);
		void	setfds(struct pollfd fds);
		Server();
		Server(int port, int password);
		Server(Server &copy);
		Server &operator=(Server &copy);
		~Server(); 
		
};
int	client_message(struct pollfd &fds);
#define JOIN 11
#define OTHER 22

#endif