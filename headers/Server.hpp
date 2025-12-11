/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:07:32 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/11 14:46:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>
#include <netinet/in.h>
#include <poll.h>
#include <vector>
#include <csignal>
#include <vector>
#include <cctype>
#include "Client.hpp"
#include "RepErr.hpp"
#include "Channel.hpp"

class	Server
{
	private:
		int	port;
		int	ser;
		
		std::string	password;
		std::vector<struct pollfd> fds;
		std::vector<Client> clients;
        std::vector<Channel>           _channels;
		
	public:
		int	Create_Socket();
		int	new_client(sockaddr_in client_address, int i);
		void	erase_client(int i);
		int	return_events(sockaddr_in client_address);
		void	setfds(struct pollfd fds);
		void	setClient(Client &client);
		Server();
		Server(int port, char *password);
		Server(Server &copy);
		Server &operator=(Server &copy);
		~Server(); 
		//asmae
		Client   *getClient(std::string client);
		Channel  *getChannel(std::string channel);
		 static void        send_msg(std::string data, int fd);
		 static std::vector<std::string>  split(const std::string &str, char delimiter);
	     void               leaveAll(Client *user);
		 Channel      *getChannel(std::string name);
		 void      join(int fd, std::string data, Client *user);
		 void 	   invite(std::string data, Client client);
		 void      kick(std::string data, Client client);
		 void      topic(std::string data, Client *client);
		 void      CommandHandler(int fd, std::string &data, Client *client);
		
};

int	client_message(Client &t_client, std::vector<Client> &clients);
int	client_acess();
#endif