/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 15:58:27 by aferryat          #+#    #+#             */
/*   Updated: 2026/02/02 16:55:55 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../headers/Server.hpp"

int	flag = 0;

void	handler_signal(int	sig)
{
	if (sig == SIGINT)
		flag = 1;
}

void    starting_server(Server &ircserv)
{
    sockaddr_in         client_address;
    struct	pollfd		fds;

    fds.fd = ircserv.get_server_socket();
	fds.events = POLLIN;
	ircserv.setfds(fds);
    std::cout << "------- irc server started --------" << std::endl;
    while (true)
	{
		if (ircserv.serverProcess(client_address) == 1)
			return ;
		if (flag == 1)
			break ;
	}
}

int main(int arc, char **arv)
{
    
    if (arc != 3 || !arv[2][0] || !arv[1][0])
	    return (std::cerr << "invalid executing: should be (./ft_irc \"port\" \"password\")" << std::endl, 1);
    try
    {
        Server  ircserv(arv[1], arv[2]);
        ircserv.create_socket();
        if (listen(ircserv.get_server_socket(), 500) < 0)
        {
            close(ircserv.get_server_socket());
            throw "Error: Listen fails";
        }
        std::signal(SIGINT, handler_signal);
        starting_server(ircserv);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(const char *e)
    {
        std::cerr << e << std::endl;
    }
}