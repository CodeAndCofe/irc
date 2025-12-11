/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amandour <amandour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:21:18 by amandour          #+#    #+#             */
/*   Updated: 2025/12/10 18:07:32 by amandour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 
#include "Client.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class Channel
{
    private:
        std::string             key;
        std::string             name;
        std::string             topic;
        std::vector<Client>     admins;
        std::vector<Client>     members;
        std::vector<Client>     invited;
        bool                    invOnly;
		bool					topicMode;
        size_t                  limits;

    public:

        Channel( void );
        Channel(std::string name);

        //getters
        std::string				getKey();
        std::string				getName();
        std::vector<Client>		getAdmins();
		std::string				getTopic();
        std::vector<Client>&	getMembers();
        size_t                  getlimits();
		bool 					getInvOnlyMode();
		bool 					getTopicMode();

        //setters
        void	setKey(std::string key);
		void	setTopic(std::string topic);
        void	setAdmin(Client client);
        void    setLimits(size_t num);
		void	setInviteMode(bool mode);
		void	setTopicMode(bool mode);

        void		addNewMember(Client  client);
        void        inviteClient(Client  client);
        void        kickMember(Client  client);
        void        removeAdmin(Client  client);
        bool		memberExist(Client  client);
		bool		isAdmine(Client client);
		bool		isInvited(Client  client);
        std::string nameReply();
        static void	sendWelcomeMsg(Client client, Channel channel);
        static void sendModeMsg(Client client, Channel channel, std::string mode, std::string param);
        static void sendKickingMsg(Client sender, Channel channel, Client target, std::string comment);
};