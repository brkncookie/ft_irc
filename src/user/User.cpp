/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 21:50:38 by mbouthai          #+#    #+#             */
/*   Updated: 2023/07/25 22:31:12 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() {}

User::~User() {}

User::User(const User& instance) {}

User& User::operator=(const User& instance) {}


std::string&    User::getUsername() const {}

std::string&    User::getFullname() const {}

std::string&    User::getHostname() const {}

std::string&    User::getPassword() const {}

bool            User::isAuthenticated() const {}

bool            User::isOperator() const {}


void   User::setUsername(std::string username) {}

void    User::setFullname(std::string fullname) {}

void    User::setHostname(std::string hostname) {}

void    User::setPassword(std::string password) {}

void    User::setAuthenticated(bool authenticated) {}

void    User::setOperator(bool boolean) {}

std::ostream&	operator<<(std::ostream& outputStream, const User& user) {}
