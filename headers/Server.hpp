/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 12:07:32 by aferryat          #+#    #+#             */
/*   Updated: 2026/02/02 16:47:39 by aferryat         ###   ########.fr       */
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
#include <csignal>
#include <vector>
#include <cctype>
#include <cstddef>
#include <fcntl.h>
#include "Client.hpp"
#include "RepErr.hpp"
#include "Channel.hpp"

class	Server
{
	private:
		int							port;
		int							serv;
		std::string					password;
		std::vector<struct pollfd> 	fds;
		std::vector<Client> 		clients;
		std::vector<Channel>        _channels;
	public:
		Server();
		Server(char *port, char *password);
		Server(Server &copy);
		Server &operator=(Server &copy);
		~Server();

		// getters:
		std::string	get_password();
		int			get_port();
		int			get_server_socket();
		//server socket
		int				new_client(sockaddr_in client_address);
		void			create_socket();
		int				serverProcess(sockaddr_in &client_address);
		void			clientHandler(Client &t_client);
		void			authentication(Client &t_client);
		void     	 	leaveAll(Client *user);
		void			password_client(Client &t_client, std::string &pass);
		void 	   		invite(std::string data, Client client);
		void      		kick(std::string data, Client client);
		void      		topic(std::string data, Client *client);
		void			regester_userName(Client &t_client, std::string &user);
		void    		regester_nickName(Client &t_client, std::string &nick);
		bool			isNickNameExist(std::string mybuffer);
		void    		regester_user(Client &t_client);
		void     		privmsg(std::string data, Client user);
		void     		mode(std::string data, Client client);
		Channel  		*getChannel(std::string channel);
		void			CommandHandler(int fd, std::string &data, Client *client);
		Client   		*getClient(std::string client);
		void 			join(int fd, std::string data, Client *user);
		//set data
		void		setfds(struct pollfd fds);
		void		setClient(Client &client);
		//clear data:
		void		erase_client(int i);
		//error
		class		InvalidPortInput;
		class		InvalidPassowrdInput;
		//other uses:
		static std::vector<std::string>  split(const std::string &str, char delimiter);
		static void  					send_msg(std::string data, int fd);
};

bool    clean_compare(std::string a, std::string b);

class	Server::InvalidPortInput : public std::exception
{
	public:
		const char* what() const throw();
};

class	Server::InvalidPassowrdInput : public std::exception
{
	public:
		const char* what() const throw();
};

#endif