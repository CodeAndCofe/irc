/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:07:32 by aferryat          #+#    #+#             */
/*   Updated: 2026/01/03 17:36:37 by amandour         ###   ########.fr       */
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
#include <fcntl.h>
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
		int		user_name_regester(Client &t_client, std::string cmd);
		Client   *getClient(std::string client);
		Channel  *getChannel(std::string channel);
		int	password_client(Client &t_client, std::string cmd);
		static void        send_msg(std::string data, int fd);
		int			nick_name_regester(Client &t_client, std::string cmd);
		static 		std::vector<std::string>  split(const std::string &str, char delimiter);
	    void     	 leaveAll(Client *user);
		void     	 join(int fd, std::string data, Client *user);
		void 	   invite(std::string data, Client client);
		void      kick(std::string data, Client client);
		void      topic(std::string data, Client *client);
		void      CommandHandler(int fd, std::string &data, Client *client);
		int		client_message(Client &t_client);
		int		client_acess(Client &t_client);
		int		isNickNameExist(std::string mybuffer);
		void     privmsg(std::string data, Client user);
		void     mode(std::string data, Client client);
};

bool    clean_compare(std::string a, std::string b, size_t l);

#endif