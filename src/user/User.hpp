/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 02:34:53 by mbouthai          #+#    #+#             */
/*   Updated: 2023/08/19 11:18:55 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>

class User
{
    private:
        std::string     _username;
        std::string     _fullname;
        std::string     _hostname;
        std::string     _password;
        bool            _authenticated;
        bool            _operator;

    public:
        User();
        ~User();
        User(const User& instance);
        User& operator=(const User& instance);

        std::string&    getUsername() const;
        std::string&    getFullname() const;
        std::string&    getHostname() const;
        std::string&    getPassword() const;
        bool            isAuthenticated() const;
        bool            isOperator() const;

        void    setUsername(std::string username);
        void    setFullname(std::string fullname);
        void    setHostname(std::string hostname);
        void    setPassword(std::string password);
        void    setAuthenticated(bool authenticated);
        void    setOperator(bool boolean);
};

std::ostream&	operator<<(std::ostream& outputStream, const User& user);