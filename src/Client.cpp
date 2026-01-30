/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:16:06 by amandour          #+#    #+#             */
/*   Updated: 2026/01/30 16:39:13 by aferryat         ###   ########.fr       */
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
	regestred = false; 
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
        this->_fd = src._fd;
        this->_ipadd = src._ipadd;
        this->_username = src._username;
        this->_nickname = src._nickname;
        this->_hasPass = src._hasPass;
        this->_isRegistred = src._isRegistred;
        this->_hasUser = src._hasUser;
        this->_hasNick = src._hasNick;
        this->regestred = src.regestred; 
    }
    return *this;
}

Client::Client(const Client &src)
{
    this->_fd = src._fd;
    this->_ipadd = src._ipadd;
    this->_username = src._username;
    this->_nickname = src._nickname;
    this->_hasPass = src._hasPass;
    this->_isRegistred = src._isRegistred;
    this->_hasUser = src._hasUser;
    this->_hasNick = src._hasNick;
	this->regestred = src.regestred;
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

void        Client::setBuffer(std::string buffer)
{
   if (_buffer.empty())
        _buffer = buffer;
    else
        _buffer.append(buffer);
}

void     Client::empty_buffer()
{
    _buffer = "";
}

void		Client::setRegesterd(bool val)
{
	regestred = val;
}

bool		Client::getRegestred()
{
	return (this->regestred);
}

