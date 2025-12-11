/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:16:06 by amandour          #+#    #+#             */
/*   Updated: 2025/12/10 15:39:01 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Client.hpp"

Client::Client()
{
    _fd = 0;
    _ipadd = "";
    _username = "";
    _nickname = "";
    _hasPass = false;
    _isRegistred = false;
    _hasUser = false;
    _hasNick = false;  
}

Client::~Client()
{
}

Client::Client(int fd, std::string ipadd, std::string nick, std::string user)
{
    _fd = fd;
    _ipadd = ipadd;
    _username = user;
    _nickname = nick;
}

Client &Client::operator=(const Client &src)
{
    if (this != &src)
    {
        _fd = src._fd;
        _ipadd = src._ipadd;
        _username = src._username;
        _nickname = src._nickname;
    }
    return *this;
}

Client::Client(const Client &src)
{
    *this = src;
}

int            Client::getFd() const
{
    return _fd;
}

std::string    Client::getIpadd() const
{
    return _ipadd;
}

std::string &Client::getNickname()
{
    return _nickname;
}

std::string &Client::getUserName()
{
    return _username;
}

bool           Client::getHaspass() const
{
    return _hasPass;
}

void        Client::setFd(int fd)
{
    _fd = fd;
}

void        Client::setIpadd(std::string ipadd)
{
    _ipadd = ipadd;
}

void    Client::setUsername(std::string user_name)
{
    _username = user_name;
}

void Client::setNickname(std::string nick_name)
{
    _nickname = nick_name;
}

void Client::setHaspass(bool hasPass)
{
    _hasPass = hasPass;
}

void Client::setIsRegistred(bool is_registred)
{
    _isRegistred = is_registred;
}

bool Client::getIsRegistred() const
{
    return _isRegistred;
}

void Client::setHasUser(bool has_user)
{
    _hasUser = has_user;
}

bool Client::getHasUser() const
{
    return _hasUser;
}

void Client::setHasNick(bool has_nick)
{
    _hasNick = has_nick;
}

bool Client::getHasNick() const
{
    return _hasNick;
}

std::string &Client::getBuffer()
{
    return _buffer;
}