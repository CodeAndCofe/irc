/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 15:15:31 by aferryat          #+#    #+#             */
/*   Updated: 2026/02/02 15:21:10 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"

bool    clean_compare(std::string a, std::string b)
{
    size_t  l = b.length();
    if (strncmp(a.c_str(), b.c_str(), l) == 0)
        return (true);
    return (false);
}

void    Server::send_msg(std::string data, int fd)
{
    send(fd, data.c_str(), data.size(), MSG_DONTWAIT);
}