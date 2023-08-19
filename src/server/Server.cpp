/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouthai <mbouthai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 04:11:17 by mbouthai          #+#    #+#             */
/*   Updated: 2023/07/26 03:22:59 by mbouthai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>

Server::Server() 
    : _name("default"),
    _password("default"),
    _port(6667)
{}

Server::~Server() {}

Server::Server(const Server& instance)
    : _name(instance.getName()),
    _password(instance.getPassword()),
    _port(instance.getPort())
{}

Server& Server::operator=(const Server& instance)
{
    if (this == &instance)
        return (*this);
    return (*this);
}

const std::string&    Server::getName() const
{
    return (this->_name);
}

const std::string&    Server::getPassword() const
{
    return (this->_password);
}

const size_t  Server::getPort() const
{
    return (this->_port);
}

std::vector<User *>&  Server::getUsers()
{
    return (this->_users);
}

std::vector<Channel *>&    Server::getChannels()
{
    return (this->_channels);
}


void    Server::configure()
{
    sockaddr_in address;
    pollfd      poll;
    int         bindStatus;
    int         listenStatus;    
    int         maxUsers = 15;
    // Creation socket for the server and check it's status 
    poll.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (poll.fd < 0)
        (std::cerr << "Error : Creation of Socket Failed !" << std::endl, exit(EXIT_FAILURE));
    {
        int enabled = 1;
        setsockopt (poll.fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled) );
    }// Set the socket to non-blocking mode
    fcntl(poll.fd, F_SETFL, O_NONBLOCK);
    
    // Filling the struct members with data 
    address.sin_family      = AF_INET;
    address.sin_port        = htons(this->_port);
    address.sin_addr.s_addr = INADDR_ANY;
    
    //  Set the events member of poll_fd to monitor for input and output events
    poll.events  = POLLIN | POLLOUT;
    poll.revents = 0;

    // Bind the socket to the IP address and port of the server and check the status 
    bindStatus = bind(poll.fd, (struct sockaddr *)&address, sizeof(address));
    if (bindStatus < 0)
        (std::cerr << "Error : Binding Failed !" << std::endl, exit(EXIT_FAILURE));
    
    // Listing for incoming connection requests and check the status
    listenStatus = listen(poll.fd, maxUsers);
    if (listenStatus < 0)
        (std::cerr << "Error : Listing Failed !" << std::endl, exit(EXIT_FAILURE));
    
    // push the SocketFd struct to the victor
    this->_fds.push_back(socketFd);
}

std::ostream&	operator<<(std::ostream& outputStream, const Server& server) {}
