/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:21:07 by amandour          #+#    #+#             */
/*   Updated: 2025/12/26 15:58:34 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <arpa/inet.h>
class Client
{
    private:
        std::string     client_ip_address;
		bool			regestred;
        int        		_fd;
        std::string		_ipadd;
        std::string		_nickname;
        std::string		_username;
        bool       		_hasPass;
        bool       		_isRegistred;
        bool       		_hasUser;
        bool       		_hasNick;
        std::string		_buffer;
    public:

        Client();
        Client(int fd, std::string ipadd, std::string nick, std::string user);
        Client(const Client &src);
        Client &operator=(const Client &src);
        ~Client();

        // ip_client for prefix

        void    create_ip(sockaddr_in &client_address);


        //****** getters
        int            getFd() const;
        std::string    getIpadd() const;
        std::string    &getUserName();
        std::string    &getNickname();
        bool           getHaspass() const;
        bool           getIsRegistred() const;
        bool           getHasUser() const;
        bool           getHasNick() const;
		bool			getRegestred();
        std::string    &getBuffer();
        std::string    getClientIp();

        //****** setters 
        void        setFd(int fd);
        void        setIpadd(std::string ipadd);
        void        setUsername(std::string username);
        void        setNickname(std::string nickname);
        void        setHaspass(bool hasPass);
        void        setIsRegistred(bool isregistred);
        void        setHasUser(bool hasuser);
        void        setBuffer(std::string buffer);
        void        setHasNick(bool hasnick);
		void		setRegesterd(bool val);
}; 