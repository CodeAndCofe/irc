/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aferryat <aferryat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:17:37 by aferryat          #+#    #+#             */
/*   Updated: 2025/12/26 15:22:49 by aferryat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Server.hpp"


bool    clean_compare(std::string a, std::string b, size_t l)
{
    if (strncmp(a.c_str(), b.c_str(), l) == 0)
        return true;
    return (false);
}